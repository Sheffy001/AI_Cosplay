aux_source_directory(/home/sheffy/CppProject/AIChat/aichat/src SRC_LIST)
add_library(aichat ${SRC_LIST})

target_link_libraries(aichat pthread muduo_net muduo_base  mysqlclient tools http)