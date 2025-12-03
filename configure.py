from pathlib import Path
import subprocess

ROOT_PATH = Path(__file__).parent

class Module:
    name: str
    executable: bool
    global_dependency: bool

    def __init__(self, name: str, executable: bool, global_dependency: bool) -> None:
        self.name = name
        self.executable = executable
        self.global_dependency = global_dependency

def list_modules() -> list[Module]:
    modules: list[Module] = []
    src_path = ROOT_PATH / 'src'
    for entry in src_path.iterdir():
        if entry.is_dir():
            sources = [file for file in entry.iterdir() if file.is_file() and file.suffix in ['.cpp', '.c']]
            if sources:
                modules.append(Module(
                    name=entry.name,
                    executable="main.cpp" in [file.name for file in sources],
                    global_dependency=entry.name.startswith('_')
                ))
    return modules

if __name__ == '__main__':
    top_level_meson_build_content = f"project('{ROOT_PATH.name}', 'cpp', default_options : ['cpp_std=c++20'],)\n"
    modules = list_modules()
    global_deps = [module for module in modules if module.global_dependency]
    global_dep_string = f", dependencies: [{', '.join(f"{module.name}_dep" for module in global_deps)}]" if global_deps else ""
    for module in modules:
        print(f'Module: {module.name}')
        sources: list[Path] = []
        for file in (ROOT_PATH / 'src' / module.name).iterdir():
            if file.is_file() and file.suffix in ['.cpp', '.c'] and file.name != 'main.cpp':
                sources.append(file)
        module_meson_build_content = ""
        if sources:
            module_meson_build_content += f"{module.name}_inc = include_directories('.')\n"
            module_meson_build_content += f"{module.name}_lib = static_library('{module.name}', [" + ', '.join(f"'{src.name}'" for src in sources) + f"], include_directories: {module.name}_inc)\n"
            module_meson_build_content += f"{module.name}_dep = declare_dependency(link_with: {module.name}_lib, include_directories: {module.name}_inc)\n"
        if (ROOT_PATH / 'src' / module.name / 'main.cpp').exists():
            module_meson_build_content += f"executable('{module.name}', ['main.cpp']" + (f", link_with: [{module.name}_lib]" if sources else "") + global_dep_string + ", install: true, install_dir: join_paths(meson.project_build_root(), 'bin'))\n"
        if module_meson_build_content:
            with open(ROOT_PATH / 'src' / module.name / 'meson.build', 'w') as f:
                f.write(module_meson_build_content)
            top_level_meson_build_content += f"subdir('src/{module.name}')\n"
        else:
            print(f'Warning: No sources found for module {module.name}, skipping meson.build generation.')
    with open(ROOT_PATH / 'meson.build', 'w') as f:
        f.write(top_level_meson_build_content)
    print('Running meson setup...')
    subprocess.run(['meson', 'setup', '--reconfigure', 'builddir', '-Dbuildtype=release', '-Doptimization=3', '-Ddebug=false', '-Db_lto=true', '-Db_pie=true'], check=True)