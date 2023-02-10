cmake_minimum_required(VERSION 3.18.4)
project(ve3ysh_util)

set(CMAKE_CXX_STANDARD 20)

include_directories(BetterMain File)

add_compile_options(-Wall -Wextra -pedantic -Werror -Wconversion -Wno-attributes -Wno-unknown-pragmas)

add_executable(BetterMain BMainTest.cpp BetterMain/BMain.cpp File/StringComposite.cpp)
