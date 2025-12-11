from pathlib import Path
import subprocess
import os

ROOT_PATH = Path(__file__).parent

class Module:
    name: str

    def __init__(self, name: str) -> None:
        self.name = name

class MesonConfigurator:
    top_level_meson_build_content: str
    global_dep_string: str
    global_modules: list[Module]
    lib_modules: list[Module]
    exe_modules: list[Module]

    def __init__(self):
        self.top_level_meson_build_content = f"project('{ROOT_PATH.name}', 'cpp', default_options : ['cpp_std=c++23'],)\n"
        self.list_modules()
        self.global_dep_string = f", dependencies: [{', '.join(f"{module.name}_dep" for module in self.global_modules)}]" if self.global_modules else ""
        for module in self.global_modules:
            self.top_level_meson_build_content += self.process_module(module)
        for module in self.lib_modules:
            self.top_level_meson_build_content += self.process_module(module)
        for module in self.exe_modules:
            self.top_level_meson_build_content += self.process_module(module)
        with open(ROOT_PATH / 'meson.build', 'w') as f:
            f.write(self.top_level_meson_build_content)

    def list_modules(self) -> None:
        self.global_modules = []
        self.lib_modules = []
        self.exe_modules = []
        modules: list[Module] = []
        src_path = ROOT_PATH / 'src'
        for entry in src_path.iterdir():
            if entry.is_dir():
                sources = [file for file in entry.iterdir() if file.is_file() and file.suffix in ['.cpp', '.c']]
                if sources:
                    module = Module(name=entry.name)
                    if "main.cpp" in [file.name for file in sources]:
                        self.exe_modules.append(module)
                    elif entry.name.startswith('_'):
                        self.global_modules.append(module)
                    else:
                        self.lib_modules.append(module)

    def process_module(self, module: Module) -> None:
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
            module_meson_build_content += f"executable('{module.name}', ['main.cpp']" + (f", link_with: [{module.name}_lib]" if sources else "") + self.global_dep_string + ", install: true, install_dir: join_paths(meson.project_build_root(), 'bin'))\n"
        if module_meson_build_content:
            with open(ROOT_PATH / 'src' / module.name / 'meson.build', 'w') as f:
                f.write(module_meson_build_content)
            return f"subdir('src/{module.name}')\n"
        else:
            print(f'Warning: No sources found for module {module.name}, skipping meson.build generation.')
            return ""

if __name__ == '__main__':
    configurator = MesonConfigurator()
    print('Running meson setup...')
    os.environ['CC'] = 'gcc-14'
    os.environ['CXX'] = 'g++-14'
    subprocess.run(['meson', 'setup', '--reconfigure', 'builddir', '-Dbuildtype=release', '-Doptimization=3', '-Ddebug=false', '-Db_lto=true', '-Db_pie=true'], check=True)