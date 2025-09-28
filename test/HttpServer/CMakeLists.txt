file(GLOB_RECURSE HTTP_SERVER_SRC
    "${PROJECT_SOURCE_DIR}/HttpServer/*.cpp"
)
find_package(OpenSSL REQUIRED)
find_library(MYSQLCPPCONN_LIBRARY
    NAMES mysqlcppconn mysqlcppconn8
    PATHS /usr/lib /usr/lib64 /usr/local/lib
)
add_library(http ${HTTP_SERVER_SRC})

target_link_libraries(http muduo_net muduo_base pthread ssl crypto)