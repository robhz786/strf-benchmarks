cmake_minimum_required (VERSION 3.11)

project (
  strf-benchmarks
  LANGUAGES CXX)

include(CMakePackageConfigHelpers)

if ( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/strf/CMakeLists.txt OR
     NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/fmt/CMakeLists.txt  OR
     NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/google-benchmark/CMakeLists.txt )

   message(
     SEND_ERROR
     "Building benchmarks requires externals libraries.\n"
     "Run 'git submodule update --init'"
     )
endif ()

option(BENCHMARK_ENABLE_TESTING OFF)
option(BENCHMARK_ENABLE_GTEST_TESTS OFF)

add_subdirectory(external/google-benchmark google-benchmark)
add_subdirectory(external/fmt fmt)
add_subdirectory(external/strf strf)
add_subdirectory(tools)

foreach(
    x
    to_char_ptr
    to_string
    input_float
    input_int
    tr_string
)
  # benchmarks that compare againts {fmt}

  add_executable(${x}-header-only  src/${x}.cpp)
  target_link_libraries(
    ${x}-header-only
    Strf::StrfHeaderOnly
    fmt::fmt-header-only
    benchmark )
  set_target_properties(
    ${x}-header-only
    PROPERTIES OUTPUT_NAME ${x}-header-only )

  add_executable(${x}-static-lib src/${x}.cpp)
  target_link_libraries(${x}-static-lib Strf::StrfStatic fmt::fmt benchmark)
  set_target_properties(${x}-static-lib PROPERTIES OUTPUT_NAME ${x}-static-lib)

endforeach(x)


foreach(
    x
    range
    join
    width_calculation
    utf8_to_utf16
    utf8_to_utf32
    utf16_to_utf8
    numpunct
)

  add_executable(${x}-header-only  src/${x}.cpp)
  target_link_libraries(
    ${x}-header-only
    Strf::StrfHeaderOnly
    benchmark )
  set_target_properties(
    ${x}-header-only
    PROPERTIES OUTPUT_NAME
    ${x}-header-only )

  add_executable(${x}-static-lib src/${x}.cpp)
  target_link_libraries(${x}-static-lib Strf::StrfStatic benchmark)
  set_target_properties(${x}-static-lib PROPERTIES OUTPUT_NAME ${x}-static-lib)

endforeach(x)
