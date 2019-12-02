
add_library( # Sets the name of the library.
        xhookapply

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        xhookapply.c
        )

target_link_libraries( # Specifies the target library.
        xhookapply

        # Links the target library to the log library
        # included in the NDK.
        xhook
        log )