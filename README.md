# AdventOfCode

This build use Python, Meson and Ninja. Make sure those are installed before proceeding.
For python, it is recommended to use uv and a venv.

(Windows only) open cmd then run:
```
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

(All) run:
```
python configure.py
python compile.py
```

If everything went well, release binaries will be located in builddir/bin
