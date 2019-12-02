
add_library( # Sets the name of the library.
        xhook

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        xhook.c
        xh_core.c
        xh_elf.c
        xh_jni.c
        xh_log.c
        xh_util.c
        xh_version.c
        )

target_link_libraries( # Specifies the target library.
        xhook

        # Links the target library to the log library
        # included in the NDK.
        log )