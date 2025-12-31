cmake_minimum_required(VERSION 3.10)

project(TEST_FAL)

set(CMAKE_C_STANDARD 99)

include_directories(
    .               # 当前目录 
    ./fal/inc       # FAL 头文件
)

file(GLOB ALL_SRC   "./fal/src/*.c"
                    "./*.c")

add_executable(fdb_sim  ${ALL_SRC})

