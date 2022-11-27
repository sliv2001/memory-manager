cmake_minimum_required(VERSION 3.25)

project(memory-manager)

add_executable(mm_test memory-manager.c memmem.c test.c)

include(CheckFunctionExists)
check_function_exists(memmem HAVE_MEMMEM_FUNCTION)
if (HAVE_MEMMEM_FUNCTION)
	target_compile_definitions(mm_test PRIVATE _HAS_MEMMEM)
endif()

target_compile_definitions(mm_test PRIVATE $<$<CONFIG:Debug>:DEBUG>)

enable_testing()

add_test(NAME test COMMAND mm_test)
set_tests_properties(test
  PROPERTIES PASS_REGULAR_EXPRESSION "10"
)