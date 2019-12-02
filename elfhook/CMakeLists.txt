
cmake_minimum_required(VERSION 3.4.1)


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

# CmakeList.txt in other directories
#add_subdirectory(src/main/cpp/elfhook1)
#add_subdirectory(src/main/cpp/elfhook2)
#add_subdirectory(src/main/cpp/elfhook3)
add_subdirectory(src/main/cpp/xHook)
add_subdirectory(src/main/cpp/xhookapply)