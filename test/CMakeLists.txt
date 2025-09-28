# 搜集 HttpServer 源码
file(GLOB_RECURSE HTTP_SERVER_SRC
    "${PROJECT_SOURCE_DIR}/HttpServer/*.cpp"
)

# 查找 OpenSSL
find_package(OpenSSL REQUIRED)

# 查找 MySQL C++ Connector 库
find_library(MYSQLCPPCONN_LIBRARY
    NAMES mysqlcppconn mysqlcppconn8
    PATHS /usr/lib /usr/lib64 /usr/local/lib
)
include_directories(${PROJECT_SOURCE_DIR}/HttpServer/include)

# 构建 HttpServer 静态库
add_library(http ${HTTP_SERVER_SRC})

# 链接依赖库
target_link_libraries(http
    muduo_net
    muduo_base
    pthread
    OpenSSL::SSL
    OpenSSL::Crypto
    ${MYSQLCPPCONN_LIBRARY}
)

# 添加 HttpServer 子目录
add_subdirectory(${PROJECT_SOURCE_DIR}/HttpServer)

# 测试可执行文件
add_executable(tests testssl.cc)

# 链接测试程序依赖
target_link_libraries(tests
    http
   
)

