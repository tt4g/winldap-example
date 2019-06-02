cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_USER_MAKE_RULES_OVERRIDE
  "${CMAKE_CURRENT_LIST_DIR}/cmake/CmakeUserRulesOverride.cmake")

set(LDAP_SEARCH_TARGET_WINDOWS_SDK_VERSION 10.0.17134.0)

# Set `CMAKE_SYSTEM_VERSION` if not defined.
# `CMAKE_SYSTEM_NAME` copy from `CMAKE_HOST_SYSTEM_NAME` if not defined.
#
# * https://cmake.org/cmake/help/latest/variable/CMAKE_SYSTEM_NAME.html
# * https://cmake.org/cmake/help/latest/variable/CMAKE_SYSTEM_VERSION.html
if(NOT DEFINED CMAKE_SYSTEM_VERSION
  AND ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows"
    OR "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows"))

  set(CMAKE_SYSTEM_VERSION ${LDAP_SEARCH_TARGET_WINDOWS_SDK_VERSION}
    CACHE INTERNAL "" FORCE)
endif()

project(LDAP_SEARCH VERSION 1.0.0 LANGUAGES CXX)

option(LDAP_SEARCH_USING_CONAN "Use conan to install dependent libraries" ON)
option(LDAP_SEARCH_BUILD_TEST "Build test" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(LDAP_SEARCH_USING_CONAN)
################################################################################
# cmake-conan
# https://github.com/conan-io/cmake-conan

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.14/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include("${CMAKE_BINARY_DIR}/conan.cmake")

conan_cmake_run(
  CONANFILE conanfile.txt
  BASIC_SETUP
  BUILD missing)

# cmake-conan
################################################################################

  set(CATCH2_CMAKE_MODULE_PATH ${CONAN_CMAKE_MODULE_PATH})
endif()

find_package(Boost 1.70.0 EXACT REQUIRED)
find_package(Wldap32 ${LDAP_SEARCH_TARGET_WINDOWS_SDK_VERSION} EXACT REQUIRED)

function(add_target_definitions target)
  target_compile_options(${target}
    PRIVATE
      $<$<CXX_COMPILER_ID:MSVC>:/W4>
      $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<VERSION_GREATER_EQUAL:${MSVC_VERSION},1900>>:/permissive->
      $<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>
      $<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>
      $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic -Werror>
      $<$<CXX_COMPILER_ID:GNU>:--input-charset=utf-8>
      $<$<CXX_COMPILER_ID:GNU>:--exec-charset=utf-8>)
endfunction()

add_library(LDAP_SEARCH_LIB STATIC
  src/ldap/ldap_error.cpp
  src/ldap/ldap_result.cpp
  src/ldap/ldap_system_error.cpp)

add_target_definitions(LDAP_SEARCH_LIB)

target_include_directories(LDAP_SEARCH_LIB
  PUBLIC
    # $<INSTALL_INTERFACE:include/ldap-search
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(LDAP_SEARCH_LIB
  PUBLIC
    Boost::boost
    Wldap32::Wldap32)

add_executable(LDAP_SEARCH main.cpp)

add_target_definitions(LDAP_SEARCH)

target_link_libraries(LDAP_SEARCH
  PUBLIC
    LDAP_SEARCH_LIB)

if(LDAP_SEARCH_BUILD_TEST)
  enable_testing()
  add_subdirectory(test)
endif()
