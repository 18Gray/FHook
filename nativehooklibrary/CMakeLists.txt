cmake_minimum_required(VERSION 3.4.1)
project(NativeHook)

# The version number.
set (NativeHook_VERSION_MAJOR 1)
set (NativeHook_VERSION_MINOR 0)


# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable


add_library(main SHARED src/main/cpp/main.cpp)


#add_executable( inject ${SOURCE_INJECT_FILES})
#add_executable( inject2 src/main/cpp/inject/inject2.c)
find_library( log-lib log )


target_link_libraries(main ${log-lib})
