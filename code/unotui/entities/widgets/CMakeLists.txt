target_sources_local(
  ${APP_LIBRARY_NAME}
  PUBLIC
    widget.h
    
    w_buffer.h
    w_button.h
    w_editabletext.h
    w_genericscrollbox.h
    w_horizontalbox.h
    w_editabletextbox.h
    w_overlay.h
    w_rectangle.h
    w_scrollbar.h
    w_tab.h
    w_text.h
    w_textbox.h
    w_textscrollbox.h
    w_verticalbox.h
    w_widgethierarchybrowser.h
  PRIVATE
    widget.cpp
    
    w_buffer.cpp
    w_button.cpp
    w_editabletext.cpp
    w_genericscrollbox.cpp
    w_horizontalbox.cpp
    w_editabletextbox.cpp
    w_overlay.cpp
    w_rectangle.cpp
    w_scrollbar.cpp
    w_tab.cpp
    w_text.cpp
    w_textbox.cpp
    w_textscrollbox.cpp
    w_verticalbox.cpp
    w_widgethierarchybrowser.cpp
)

add_subdirectory( geometry )
