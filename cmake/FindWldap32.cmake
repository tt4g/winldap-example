cmake_minimum_required(VERSION 3.4.0)

#[=======================================================================[.rst:
FindWldap32
-----------

Find the Wldap32 libraries from Windows SDK.

Override Windows SDK version
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Get the version of Windows SDK from the variable
`CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION`.
If `CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION` is not specified, get from the
variable `CMAKE_SYSTEM_VERSION`.

.. note::

  `CMAKE_SYSTEM_VERSION` must be specified `cmake -DCMAKE_SYSTEM_VERSION=...`.
  Otherwise, specify this before `project()` is called in `CMakeLists.txt`.

  Set `CMAKE_SYSTEM_VERSION` example:

  .. code-block:: cmake

      # Add a directry path containing FindWldap32.cmake
      list(APPEND CMAKE_MODULE_PATH cmake)

      set(TARGET_WINDOWS_SDK_VERSION 10.0.17763.0)

      if(NOT DEFINED CMAKE_SYSTEM_VERSION
        AND ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows"
          OR "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows"))

        set(CMAKE_SYSTEM_VERSION ${TARGET_WINDOWS_SDK_VERSION}
          CACHE INTERNAL "" FORCE)
      endif()

      project(example-project)

      find_package(Wldap32 ${TARGET_WINDOWS_SDK_VERSION} EXACT REQUIRED)

IMPORTED targets
^^^^^^^^^^^^^^^^

This module defines the following `IMPORTED` target: ``Wldap32::Wldap32``

Result variables
^^^^^^^^^^^^^^^^

This module will set the following variables if found:

``Wldap32_INCLUDE_DIRS``
  Where to find sqlite3.h, etc.

``Wldap32_LIBRARIES``
  The libraries to link against to use Wldap32.

``Wldap32_VERSION``
  Version of the Wldap32 library found, this is the version of Windows SDK.

``Wldap32_FOUND``
  TRUE if found.

#]=======================================================================]

function(__wldap32_detect_windows_sdk_dir value)
  # Custom Windows 10 SDK directory.
  # https://cmake.org/cmake/help/latest/variable/CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION.html
  if(DEFINED ENV{CMAKE_WINDOWS_KITS_10_DIR})
    set(${value} "$ENV{CMAKE_WINDOWS_KITS_10_DIR}" PARENT_SCOPE)
  endif()

  # The environment variable `WindowsSdkDir` provided by MSVC.
  if(DEFINED ENV{WindowsSdkDir})
    set(${value} "$ENV{WindowsSdkDir}" PARENT_SCOPE)
  endif()
endfunction()

function(__wldap32_detect_windows_sdk_version value)
  if(DEFINED CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
    set(${value} ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION} PARENT_SCOPE)
    return()
  endif()

  # The environment variable `WindowsSDKVersion` is not required.
  # This is because the environment variable `Lib` provided by MSVC contains the
  # directory where the library of the Windows SDK of the corresponding version
  # exists, and is included in the search range of `find_path` and `find_library`.
  #
  # if(DEFINED ENV{WindowsSDKVersion})
  #   set(__WindowsSDKVersion "$ENV{WindowsSDKVersion}")
  #   # Delete last backslash in environment variable `WindowsSDKVersion`.
  #   # e.g. 10.0.xxxxx.x\
  #   #                  ^
  #   string(REGEX REPLACE "\\\\" "" __WindowsSDKVersion "${__WindowsSDKVersion}")
  #   set(${value} ${__WindowsSDKVersion} PARENT_SCOPE)
  #
  #   return()
  # endif()

  if(WIN32)
    # If the target platform is `WIN32`, the variable `CMAKE_SYSTEM_VERSION`
    # is the target Windows version. This should match the Windows SDK version.
    set(${value} ${CMAKE_SYSTEM_VERSION} PARENT_SCOPE)
  endif()
endfunction()

function(__wldap32_detect_windows_sdk_arch value)

  if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "ARM")
    if(CMAKE_CL_64)
      set(WINDOWS_SDK_ARCH arm64)
    else()
      set(WINDOWS_SDK_ARCH arm)
    endif()
  else()
    if(CMAKE_CL_64)
      set(WINDOWS_SDK_ARCH x64)
    else()
      set(WINDOWS_SDK_ARCH x86)
    endif()
  endif()

  set(${value} ${WINDOWS_SDK_ARCH} PARENT_SCOPE)
endfunction()

__wldap32_detect_windows_sdk_dir(Wldap32_WINDOWS_SDK_DIR)
__wldap32_detect_windows_sdk_version(Wldap32_WINDOWS_SDK_VERSION)
__wldap32_detect_windows_sdk_arch(Wldap32_TARGERT_ARCH)

set(Wldap32_INCLUDE_SUFFIXES)
set(Wldap32_LIBRARY_SUFFIXES)

if(DEFINED Wldap32_WINDOWS_SDK_VERSION)
  list(APPEND Wldap32_INCLUDE_SUFFIXES
    "include/${Wldap32_WINDOWS_SDK_VERSION}/um")

  if(DEFINED Wldap32_TARGERT_ARCH)
    list(APPEND Wldap32_LIBRARY_SUFFIXES
      "Lib/${Wldap32_WINDOWS_SDK_VERSION}/um/${Wldap32_TARGERT_ARCH}")
  endif(DEFINED Wldap32_TARGERT_ARCH)

endif(DEFINED Wldap32_WINDOWS_SDK_VERSION)

find_path(Wldap32_INCLUDE_DIR
  NAMES Winldap.h
  # Since the default Windows SDK directory exists in the search path,
  # HINTS is used to detect user-defined version libraries with priority.
  HINTS ${Wldap32_WINDOWS_SDK_DIR}
  PATH_SUFFIXES
    ${Wldap32_INCLUDE_SUFFIXES})

find_library(Wldap32_LIBRARY
  NAMES Wldap32.lib
  # Since the default Windows SDK directory exists in the search path,
  # HINTS is used to detect user-defined version libraries with priority.
  HINTS ${Wldap32_WINDOWS_SDK_DIR}
  PATH_SUFFIXES
    ${Wldap32_LIBRARY_SUFFIXES})

if(DEFINED Wldap32_INCLUDE_DIR AND DEFINED Wldap32_LIBRARY)
  # Get version string from directory name (e.g. 10.0.17763.0).
  # Include path: ${WindowsSdkDir}/Include/<version>/um
  #                                        ^^^^^^^^^
  # Library path: ${WindowsSdkDir}/Lib/<version>/um/<arch>/Wldap32.lib
  #                                    ^^^^^^^^^

  get_filename_component(Wldap32_INCLUDE_DIR_VERSION
    "${Wldap32_INCLUDE_DIR}/.." ABSOLUTE)
  get_filename_component(Wldap32_INCLUDE_DIR_VERSION
    "${Wldap32_INCLUDE_DIR_VERSION}" NAME)

    get_filename_component(Wldap32_LIBRARY_DIR_VERSION
      "${Wldap32_LIBRARY}/../../.." ABSOLUTE)
      get_filename_component(Wldap32_LIBRARY_DIR_VERSION
        "${Wldap32_LIBRARY_DIR_VERSION}" NAME)

  if(Wldap32_INCLUDE_DIR_VERSION VERSION_EQUAL Wldap32_LIBRARY_DIR_VERSION)
    set(Wldap32_VERSION ${Wldap32_INCLUDE_DIR_VERSION})
  endif()

  unset(Wldap32_INCLUDE_DIR_VERSION)
  unset(Wldap32_LIBRARY_DIR_VERSION)
endif()

unset(Wldap32_WINDOWS_SDK_DIR)
unset(Wldap32_WINDOWS_SDK_VERSION)
unset(Wldap32_TARGERT_ARCH)
unset(Wldap32_INCLUDE_SUFFIXES)
unset(Wldap32_LIBRARY_SUFFIXES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Wldap32
  REQUIRED_VARS Wldap32_INCLUDE_DIR Wldap32_LIBRARY
  VERSION_VAR Wldap32_VERSION
)

if(Wldap32_FOUND)
  set(Wldap32_INCLUDE_DIRS "${Wldap32_INCLUDE_DIR}")
  set(Wldap32_LIBRARIES "${Wldap32_LIBRARY}")

  if(NOT TARGET Wldap32::Wldap32)
    add_library(Wldap32::Wldap32 UNKNOWN IMPORTED)
    set_target_properties(Wldap32::Wldap32
      PROPERTIES
        IMPORTED_LOCATION ${Wldap32_LIBRARIES}
        INTERFACE_INCLUDE_DIRECTORIES ${Wldap32_INCLUDE_DIR}
    )
  endif()
endif()

mark_as_advanced(Wldap32_INCLUDE_DIR Wldap32_LIBRARY)
