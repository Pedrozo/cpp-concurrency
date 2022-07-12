cmake_minimum_required (VERSION 3.8)

project("CPP_CONCURRENCY")

set(CMAKE_CXX_STANDARD 17) # TODO: check compatibility
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread") # TODO: cross-platform way

add_library(cpp_concurrency INTERFACE)

# if(MSVC)
#   target_compile_options(cpp_concurrency PUBLIC /W4 /WX)
# else()
#   target_compile_options(cpp_concurrency PUBLIC -Wall -Wextra -Wpedantic -Werror)
# endif()

target_include_directories(cpp_concurrency INTERFACE inc/)

add_subdirectory("demo")