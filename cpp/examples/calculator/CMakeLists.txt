cmake_minimum_required(VERSION 3.20)

project(calculator LANGUAGES CXX)

find_package(uimd CONFIG QUIET)

if(NOT TARGET uimd::runtime)
    if(DEFINED ENV{UIMD_HOME})
        set(_uimd_sdk_home "$ENV{UIMD_HOME}")
    elseif(WIN32 AND DEFINED ENV{LOCALAPPDATA})
        set(_uimd_sdk_home "$ENV{LOCALAPPDATA}/uimd")
    elseif(DEFINED ENV{HOME})
        set(_uimd_sdk_home "$ENV{HOME}/.uimd")
    endif()
    if(DEFINED _uimd_sdk_home AND EXISTS "${_uimd_sdk_home}/current")
        file(READ "${_uimd_sdk_home}/current" _uimd_current_version)
        string(STRIP "${_uimd_current_version}" _uimd_current_version)
        set(_uimd_sdk_cpp "${_uimd_sdk_home}/sdk/${_uimd_current_version}/targets/cpp")
        if(EXISTS "${_uimd_sdk_cpp}/lib/cmake/uimd/uimdConfig.cmake")
            list(PREPEND CMAKE_PREFIX_PATH "${_uimd_sdk_cpp}")
            find_package(uimd CONFIG QUIET)
        endif()
    endif()
endif()

if(NOT TARGET uimd::runtime)
    set(_uimd_local_cpp "${CMAKE_CURRENT_LIST_DIR}/../uimd/cpp")
    if(EXISTS "${_uimd_local_cpp}/CMakeLists.txt")
        set(UIMD_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(UIMD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(UIMD_INSTALL OFF CACHE BOOL "" FORCE)
        add_subdirectory("${_uimd_local_cpp}" "${CMAKE_BINARY_DIR}/_deps/uimd-build")
    endif()
endif()

if(NOT TARGET uimd::runtime)
    include(FetchContent)

    set(UIMD_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(UIMD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(UIMD_INSTALL OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        uimd
        GIT_REPOSITORY https://github.com/uimd-lang/uimd.git
        GIT_TAG v0.4.23
    )
    FetchContent_MakeAvailable(uimd)
endif()

if(NOT TARGET uimd::runtime)
    message(FATAL_ERROR "UIMD C++ runtime was not found. Install UIMD, set CMAKE_PREFIX_PATH, keep a sibling ../uimd checkout, or use a released Git tag.")
endif()

add_executable(calculator
    calculator.cpp
    calculator_ui.cpp
)

target_compile_features(calculator PRIVATE cxx_std_20)
target_link_libraries(calculator PRIVATE uimd::runtime)
