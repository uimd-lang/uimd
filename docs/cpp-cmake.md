# C++ And CMake

The C++ backend lives under:

```text
cpp/
```

Public CMake targets:

```cmake
uimd::runtime
uimd::dialogs
```

Useful options:

```cmake
UIMD_BUILD_EXAMPLES
UIMD_BUILD_TESTS
UIMD_INSTALL
```

Configure and build:

```bash
cmake -S cpp -B cpp/build
cmake --build cpp/build
ctest --test-dir cpp/build --output-on-failure
```

Downstream projects can use `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
    uimd
    GIT_REPOSITORY https://github.com/uimd-lang/uimd.git
    GIT_TAG v0.4.0
)

FetchContent_MakeAvailable(uimd)

target_link_libraries(my_app PRIVATE uimd::runtime)
```

Installed CMake package usage:

```bash
cmake -S cpp -B cpp/build
cmake --build cpp/build
cmake --install cpp/build --prefix /opt/uimd
```

```cmake
find_package(uimd CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE uimd::runtime)
```

Configure the downstream project with:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/opt/uimd
```
