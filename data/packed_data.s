        .global DATA_CONSOLAS_FONT
        .global DATA_CONSOLAS_FONT_SIZE

        .global DATA_COLORED_VERTICES_FRAG
        .global DATA_COLORED_VERTICES_FRAG_SIZE

        .global DATA_COLORED_VERTICES_VS
        .global DATA_COLORED_VERTICES_VS_SIZE

        .global DATA_TEXTURED_VERTICES_FRAG
        .global DATA_TEXTURED_VERTICES_FRAG_SIZE

        .global DATA_TEXTURED_VERTICES_VS
        .global DATA_TEXTURED_VERTICES_VS_SIZE

        .global DATA_TEXT_FRAG
        .global DATA_TEXT_FRAG_SIZE

        .global DATA_TEXT_VS
        .global DATA_TEXT_VS_SIZE

        .section .rodata
// CONSOLAS FONT
        DATA_CONSOLAS_FONT:
                .incbin "consolas.png"
        DATA_CONSOLAS_FONT_SIZE:
                .int DATA_CONSOLAS_FONT_SIZE - DATA_CONSOLAS_FONT
// COLORED VERTICES FRAG
        DATA_COLORED_VERTICES_FRAG:
                .incbin "colored_vertices.frag"
        DATA_COLORED_VERTICES_FRAG_SIZE:
                .int DATA_COLORED_VERTICES_FRAG_SIZE - DATA_COLORED_VERTICES_FRAG
// COLORED VERTICES VS
        DATA_COLORED_VERTICES_VS:
                .incbin "colored_vertices.vs"
        DATA_COLORED_VERTICES_VS_SIZE:
                .int DATA_COLORED_VERTICES_VS_SIZE - DATA_COLORED_VERTICES_VS
// TEXTURED VERTICES FRAG
        DATA_TEXTURED_VERTICES_FRAG:
                .incbin "textured_vertices.frag"
        DATA_TEXTURED_VERTICES_FRAG_SIZE:
                .int DATA_TEXTURED_VERTICES_FRAG_SIZE - DATA_TEXTURED_VERTICES_FRAG
// TEXTURED VERTICES VS
        DATA_TEXTURED_VERTICES_VS:
                .incbin "textured_vertices.vs"
        DATA_TEXTURED_VERTICES_VS_SIZE:
                .int DATA_TEXTURED_VERTICES_VS_SIZE - DATA_TEXTURED_VERTICES_VS
// TEXT FRAG
        DATA_TEXT_FRAG:
                .incbin "text.frag"
        DATA_TEXT_FRAG_SIZE:
                .int DATA_TEXT_FRAG_SIZE - DATA_TEXT_FRAG
// TEXT VS
        DATA_TEXT_VS:
                .incbin "text.vs"
        DATA_TEXT_VS_SIZE:
                .int DATA_TEXT_VS_SIZE - DATA_TEXT_VS
