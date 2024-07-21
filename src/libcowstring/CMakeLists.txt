set(target_name cowstring)

add_library(
    ${target_name} INTERFACE
    libcowstring/cowstring.hpp
)

include(CompileOptions)
set_compile_options(${target_name} INTERFACE)

target_include_directories(
    ${target_name}
    INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}
)

# testing

set(target_name_test cowstring_tests)

add_executable(${target_name_test})

target_sources(
  ${target_name_test}
  PRIVATE
    libcowstring/cowstring.test.cpp
)

set_compile_options(${target_name_test} PUBLIC)

target_link_libraries(
  ${target_name_test}
  PUBLIC
    gtest_main
    cowstring
)

target_include_directories(
  ${target_name_test}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

include(GoogleTest)
gtest_discover_tests(${target_name_test})
