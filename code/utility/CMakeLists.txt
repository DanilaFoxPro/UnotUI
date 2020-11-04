target_sources_local(
${APP_LIBRARY_NAME}
        PUBLIC
                clipboard.h
                colors.h
                fpoint.h
                general.h
                packed_data.h
                point.h
                shortcuts.h
                text.h
                unit.h
                widget.h
                window.h
        PRIVATE
                clipboard.cpp
                fpoint.cpp
                general.cpp
                point.cpp
                shortcuts.cpp
                text.cpp
                unit.cpp
                widget.cpp
                window.cpp
)

add_subdirectory( classes )
add_subdirectory( deps )
