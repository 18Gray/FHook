cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
        main3

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        main.cpp
        elf_common.cpp
        elf_hooker.cpp
        elf_module.cpp
         )




