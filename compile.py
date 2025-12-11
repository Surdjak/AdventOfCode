import subprocess
from pathlib import Path
import os

ROOT_PATH = Path(__file__).parent

if __name__ == '__main__':
    if not (ROOT_PATH / 'builddir').exists():
        raise RuntimeError('Build directory does not exist. Please run configure.py first.')
    # Set CXX environment variable for Meson
    print('Running meson compile...')
    subprocess.run(['meson', 'compile', '-C', 'builddir'], check=True)
    subprocess.run(['meson', 'install', '-C', 'builddir'], check=True)
    print('Build and installation complete.')