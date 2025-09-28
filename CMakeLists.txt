cmake_minimum_required(VERSION 3.0)
project(raftfile)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")
#项目可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#项目库文件输出路径    
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# include_directories(
#     ${PROJECT_SOURCE_DIR}
#     ${PROJECT_SOURCE_DIR}/include
#     ${PROJECT_SOURCE_DIR}/HttpServer/include
#     ${OPENSSL_INCLUDE_DIR}
# )

include_directories(${PROJECT_SOURCE_DIR}/aichat/include)
include_directories(${PROJECT_SOURCE_DIR}/tools/include)
include_directories(${PROJECT_SOURCE_DIR}/HttpServer/include
${OPENSSL_INCLUDE_DIR})

link_directories(${PROJECT_SOURCE_DIR}/lib)
add_subdirectory(tools)
add_subdirectory(HttpServer)
add_subdirectory(aichat)