add_subdirectory(./src)
add_executable(aiserver server.cc)
target_link_libraries(aiserver http tools aichat)