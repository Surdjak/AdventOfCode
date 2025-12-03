from pathlib import Path
import os
import subprocess

BUILD_PY_PATH = Path(__file__).parent

class Module:
    name: str

    def __init__(self, name: str, executable: bool):
        self.name = name

def list_modules() -> list[Module]:
    modules: list[Module] = []
    src_path = BUILD_PY_PATH / 'src'
    for entry in sorted(os.scandir(src_path), key=lambda e: e.name):
        if entry.is_dir() and any(os.scandir(entry.path)):
            modules.append(Module(
                name=entry.name,
                executable=(Path(entry.path) / 'main.cpp').exists()
            ))
    return modules

if __name__ == '__main__':
    top_level_meson_build_content = f"project('{BUILD_PY_PATH.name}', 'cpp', default_options : ['cpp_std=c++20'],)\n"
    for module in list_modules():
        print(f'Module: {module.name}')
        sources: list[Path] = []
        # headers: list[Path] = []
        for file in (BUILD_PY_PATH / 'src' / module.name).iterdir():
            if file.is_file() and file.suffix in ['.cpp', '.c'] and file.name != 'main.cpp':
                sources.append(file)
            # elif file.is_file() and file.suffix in ['.h', '.hpp']:
            #     headers.append(file)
        module_meson_build_content = ""
        if sources:
            module_meson_build_content += f"{module.name}_inc = include_directories('.')\n"
            module_meson_build_content += f"{module.name}_lib = static_library('{module.name}', [" + ', '.join(f"'{src.name}'" for src in sources) + f"], include_directories: {module.name}_inc)\n"
            module_meson_build_content += f"{module.name}_dep = declare_dependency(link_with: {module.name}_lib, include_directories: {module.name}_inc)\n"
        if (BUILD_PY_PATH / 'src' / module.name / 'main.cpp').exists():
            module_meson_build_content += f"executable('{module.name}', ['main.cpp']" + (f", link_with: [{module.name}_lib]\n" if sources else "") + ", dependencies: [_utils_dep]" + ")\n"
        if module_meson_build_content:
            with open(BUILD_PY_PATH / 'src' / module.name / 'meson.build', 'w') as f:
                f.write(module_meson_build_content)
            top_level_meson_build_content += f"subdir('src/{module.name}')\n"
        else:
            print(f'Warning: No sources found for module {module.name}, skipping meson.build generation.')
    with open(BUILD_PY_PATH / 'meson.build', 'w') as f:
        f.write(top_level_meson_build_content)
    print('Running meson setup...')
    subprocess.run(['meson', 'setup', '--reconfigure', 'builddir'], check=True)