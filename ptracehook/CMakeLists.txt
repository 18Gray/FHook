
cmake_minimum_required(VERSION 3.4.1)


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

add_subdirectory(src/main/cpp/ptracehook/inlinehook)
add_subdirectory(src/main/cpp/ptracehook/ptracehook1)
add_subdirectory(src/main/cpp/ptracehook/ptracehook3)