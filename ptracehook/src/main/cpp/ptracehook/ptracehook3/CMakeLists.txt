
add_library( # Sets the name of the library.
        inject

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        Inject.c
        Log.c
        Tools.c
        Vector.c
        )

target_link_libraries( # Specifies the target library.
        inject

        # Links the target library to the log library
        # included in the NDK.
        log )