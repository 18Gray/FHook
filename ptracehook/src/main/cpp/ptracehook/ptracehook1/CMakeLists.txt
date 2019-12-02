
add_library( # Sets the name of the library.
        main

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        main.cpp
        tracer.cpp
        tools.cpp
        )

target_link_libraries( # Specifies the target library.
        main

        # Links the target library to the log library
        # included in the NDK.
        log )