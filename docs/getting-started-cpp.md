# Getting Started With C++

Create a minimal C++ UIMD project:

```bash
mkdir hello-uimd-cpp
cd hello-uimd-cpp
python3 -m pip install uimd

uimd new hello --target cpp
uimd generate hello.uimd --target cpp
cmake -S . -B build
cmake --build build
```

The `uimd` command is installed from the Python package even for C++ projects.
For C++, it is a generator/build tool dependency, not a Python runtime
dependency of the final executable. A C++ application links against the C++
runtime through CMake targets.

If you already have a single `hello.uimd` file, `uimd generate hello.uimd --target cpp`
creates the missing `hello.cpp` and `CMakeLists.txt` bootstrap files. Existing
app-owned files are not overwritten.

The generated `CMakeLists.txt` first tries `find_package(uimd CONFIG QUIET)`,
then a local sibling checkout at `../uimd/cpp`, and only then falls back to
fetching UIMD from the official GitHub repository with CMake `FetchContent`.
The GitHub fallback requires a published release tag.

For local development against a manually installed C++ runtime:

```bash
cmake -S path/to/uimd/cpp -B path/to/uimd/cpp/build
cmake --build path/to/uimd/cpp/build
cmake --install path/to/uimd/cpp/build --prefix /path/to/uimd-install

cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/uimd-install
cmake --build build
```

On Visual Studio generators, the executable may be under a configuration
directory such as:

```text
build\Debug\hello.exe
```

The generated `CMakeLists.txt` uses the public CMake target:

```cmake
uimd::runtime
```
