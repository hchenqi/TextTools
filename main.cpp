#include <hb.h>
#include <iostream>

int main() {
    const char* text = "Hello, HarfBuzz!";

	hb_buffer_t* buffer = hb_buffer_create();
    hb_buffer_add_utf8(buffer, text, -1, 0, -1);
    hb_buffer_guess_segment_properties(buffer);

    hb_blob_t *blob = hb_blob_create_from_file("C:\\Windows\\Fonts\\arial.ttf");
    hb_face_t *face = hb_face_create(blob, 0);
    hb_font_t *font = hb_font_create(face);

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

    hb_font_destroy(font);
    hb_face_destroy(face);
    hb_blob_destroy(blob);
    hb_buffer_destroy(buffer);

    return 0;
}
