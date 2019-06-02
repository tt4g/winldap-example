# Overview

LDAP search example.

## Warning

This source only works on Windows OS.

## Build

```bash
$ mkdir build
$ cd build
$ cmake ../
$ cmake --build .
```

### Requirements

* CMake
* Windows 10 SDK
* Conan (optional)
* Boost library (optional)
* Catch2 (optional)

### CMake variables

* LDAP_SEARCH_TARGET_WINDOWS_SDK_VERSION

    Windows SDK version.

* CATCH2_CMAKE_MODULE_PATH

    The location of `Catch2.cmake` provided by Catch2.

* LDAP_SEARCH_USING_CONAN

    If `ON`, install dependent libraries Boost and Catch2. The default is `ON`.

* LDAP_SEARCH_BUILD_TEST

    If `ON`, build test target. The default is `ON`.
