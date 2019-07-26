
cmake_minimum_required(VERSION 3.4.1)
#project (Tutorial)
# 版本号 1.0
#set (Tutorial_VERSION_MAJOR 1)
#set (Tutorial_VERSION_MINOR 0)
#add_executable(Tutorial tutorial.cxx)

add_library( # Sets the name of the library.
        shmemlib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/shm/shmem.cpp
        )


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)


target_link_libraries( # Specifies the target library.
        shmemlib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})