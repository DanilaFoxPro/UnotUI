target_sources_local(
${APP_LIBRARY_NAME}
        PUBLIC
                clipboard.h
                colors.h
                coordinates.h
                font_symbols.h
                fpoint.h
                general.h
                packed_data.h
                point.h
                shortcuts.h
                text.h
                types.h
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
                types.cpp
                unit.cpp
                widget.cpp
                window.cpp
)

add_subdirectory( classes )
add_subdirectory( deps )
