cmake_minimum_required(VERSION 3.10.0)

if(DEFINED CATCH2_CMAKE_MODULE_PATH)
  # `CATCH2_CMAKE_MODULE_PATH` is the location of Catch2.cmake.
  list(APPEND CMAKE_MODULE_PATH ${CATCH2_CMAKE_MODULE_PATH})
endif()

find_package(Catch2 2.5.0 EXACT REQUIRED)

include(CTest)
include(Catch)
enable_testing()

add_executable(LDAP_SEARCH_TEST
  test_runner.cpp
  src/dummy.cpp)

target_link_libraries(LDAP_SEARCH_TEST
  PUBLIC
    LDAP_SEARCH_LIB
    Catch2::Catch2)

add_target_definitions(LDAP_SEARCH_TEST)

catch_discover_tests(LDAP_SEARCH_TEST
  TEST_PREFIX "LDAP_SEARCH_TEST."
  WORKING_DIRECTORY $<TARGET_FILE_DIR:LDAP_SEARCH_TEST>)
