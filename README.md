# Overview

Winldap example.

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

### CMake variables

* LDAP_SEARCH_TARGET_WINDOWS_SDK_VERSION

    Windows SDK version.

* LDAP_SEARCH_USING_CONAN

    If `ON`, install dependent libraries Boost and Catch2. The default is `ON`.
