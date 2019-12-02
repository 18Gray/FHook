cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
        main1

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        main.cpp
        elfhook.cpp
        elfio.cpp
        elfutils.cpp
        )

