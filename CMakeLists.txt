cmake_minimum_required(VERSION 3.25)

project(memory-manager)

add_executable(mm_test memory-manager.c test.c)

enable_testing()

add_test(NAME test COMMAND mm_test)
set_tests_properties(test
  PROPERTIES PASS_REGULAR_EXPRESSION "10"
)