cmake_minimum_required(VERSION 3.10.0)

set(ldap-search_TARGET_WINDOWS_SDK_VERSION 10.0.17134.0)

# Set `CMAKE_SYSTEM_VERSION` if not defined.
# `CMAKE_SYSTEM_NAME` copy from `CMAKE_HOST_SYSTEM_NAME` if not defined.
#
# * https://cmake.org/cmake/help/latest/variable/CMAKE_SYSTEM_NAME.html
# * https://cmake.org/cmake/help/latest/variable/CMAKE_SYSTEM_VERSION.html
if(NOT DEFINED CMAKE_SYSTEM_VERSION
  AND ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows"
    OR "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows"))

  set(CMAKE_SYSTEM_VERSION ${ldap-search_TARGET_WINDOWS_SDK_VERSION}
    CACHE INTERNAL "" FORCE)
endif()

project(ldap-search VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Wldap32 ${ldap-search_TARGET_WINDOWS_SDK_VERSION} EXACT REQUIRED)

add_executable(ldap-search
  main.cpp
)

target_include_directories(ldap-search
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#    $<INSTALL_INTERFACE:include/ldap-search>
)

target_link_libraries(ldap-search
  PUBLIC
    Wldap32::Wldap32
)

target_compile_options(ldap-search
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<VERSION_GREATER_EQUAL:${MSVC_VERSION},1900>>:/permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)
