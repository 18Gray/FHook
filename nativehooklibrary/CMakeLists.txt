cmake_minimum_required(VERSION 3.4.1)

add_library(main SHARED src/main/cpp/main.cpp)


#add_executable( inject ${SOURCE_INJECT_FILES})
#add_executable( inject2 src/main/cpp/inject/inject2.c)
find_library( log-lib log )


target_link_libraries(main ${log-lib})
