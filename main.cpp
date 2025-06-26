#include <hb.h>
#include <hb-ft.h>
#include <iostream>

int main() {
    const char* text = "Hello, HarfBuzz!";

	hb_buffer_t* buffer = hb_buffer_create();
    hb_buffer_add_utf8(buffer, text, -1, 0, -1);
    hb_buffer_guess_segment_properties(buffer);

	FT_Library ft_library;
    if (FT_Init_FreeType(&ft_library)) {
        std::cerr << "Failed to init FreeType\n";
        return 1;
    }
    FT_Face ft_face;
    if (FT_New_Face(ft_library, "C:\\Windows\\Fonts\\arial.ttf", 0, &ft_face)) {
        std::cerr << "Failed to load font\n";
        return 1;
    }
    FT_Set_Char_Size(ft_face, 0, 16 * 64, 300, 300);
    hb_font_t* font = hb_ft_font_create(ft_face, nullptr);

    hb_shape(font, buffer, nullptr, 0);

    unsigned int count;
    hb_glyph_info_t* info = hb_buffer_get_glyph_infos(buffer, &count);
    hb_glyph_position_t* pos = hb_buffer_get_glyph_positions(buffer, &count);

    std::cout << "Shaped glyphs:\n";
    for (unsigned int i = 0; i < count; ++i) {
        std::cout << "glyph " << info[i].codepoint
                  << " at x_advance=" << pos[i].x_advance
                  << ", y_advance=" << pos[i].y_advance << "\n";
    }

    hb_buffer_destroy(buffer);
    hb_font_destroy(font);
    FT_Done_Face(ft_face);
    FT_Done_FreeType(ft_library);

    return 0;
}
