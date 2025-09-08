# CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(ThreadPool VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(MSVC)
    # Windows下的MSVC编译器选项
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
else()
    # GCC/Clang编译器选项
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
endif()

# 查找pthread库
find_package(Threads REQUIRED)

# 添加头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 创建ThreadPool库
add_library(ThreadPoolLib
    ThreadPool.cpp
    ThreadPool.h
)

# 为ThreadPool库链接pthread
target_link_libraries(ThreadPoolLib Threads::Threads)

# 创建可执行文件
add_executable(ThreadPoolDemo
    main.cpp
)

# 链接ThreadPool库到可执行文件
target_link_libraries(ThreadPoolDemo ThreadPoolLib)

# 设置可执行文件输出目录
set_target_properties(ThreadPoolDemo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 可选：如果要安装库和头文件
install(TARGETS ThreadPoolLib ThreadPoolDemo
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ThreadPool.h
    DESTINATION include
)

# 可选：添加调试信息
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
    if(NOT MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()
endif()

# 打印一些有用的信息
message(STATUS "CMake version: ${CMAKE_VERSION}")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}") 