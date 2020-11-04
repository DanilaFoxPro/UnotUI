target_sources_local(
  ${APP_LIBRARY_NAME}
  PRIVATE
    packed_data.s
)

target_include_directories(
  ${APP_LIBRARY_NAME}
  PRIVATE
    .
    fonts
    shader_source
)
