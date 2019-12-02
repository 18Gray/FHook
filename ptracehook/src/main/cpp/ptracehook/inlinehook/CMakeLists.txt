
add_library( # Sets the name of the library.
        Ihook

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        Ihook.c
        )

target_link_libraries( # Specifies the target library.
        Ihook

        # Links the target library to the log library
        # included in the NDK.
        log )