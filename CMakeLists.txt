cmake_minimum_required(VERSION 3.10.0)

project(ldap-search VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(ldap-search_LINK_LIBRARYES)
set(ldap-search_SYSTEM_SUFFIX)

if (WIN32)
  set(ldap-search_SYSTEM_SUFFIX win)

  add_library(Wldap32 SHARED IMPORTED)

  list(APPEND ldap-search_LINK_LIBRARYES Wldap32)
else()
  message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR} is not supported!")
endif()

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
    ${ldap-search_LINK_LIBRARYES}
)

target_compile_options(ldap-search
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<VERSION_GREATER_EQUAL:${MSVC_VERSION},1900>>:/permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)
