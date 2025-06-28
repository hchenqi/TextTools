#include <WndDesign/window/Global.h>
#include <WndDesign/widget/TitleBarFrame.h>
#include <WndDesign/control/Placeholder.h>

#include <WndDesign/system/d2d_api.h>
#include <WndDesign/system/directx_helper.h>

#include <hb.h>
#include <hb-directwrite.h>
#include <iostream>


using namespace WndDesign;


class HBFont {
private:
	hb_blob_t* blob;
	hb_face_t* face;
	hb_font_t* font;
public:
	HBFont(const char* file) {
		blob = hb_blob_create_from_file(file);
		face = hb_face_create(blob, 0);
		font = hb_font_create(face);
	}
	~HBFont() {
		hb_font_destroy(font);
		hb_face_destroy(face);
		hb_blob_destroy(blob);
	}
public:
	hb_face_t* get_face() const { return face; }
	hb_font_t* get_font() const { return font; }
};

class GlyphRun {
private:
	friend struct GlyphRunFigure;
	DWRITE_GLYPH_RUN data;
	std::vector<UINT16> glyph_indices;
	std::vector<FLOAT> advances;
	std::vector<DWRITE_GLYPH_OFFSET> offsets;

public:
	GlyphRun(const HBFont& font, float font_size, const char* text) {
		hb_buffer_t* buffer = hb_buffer_create();
		hb_buffer_add_utf8(buffer, text, -1, 0, -1);
		hb_buffer_guess_segment_properties(buffer);

		hb_font_set_scale(font.get_font(), font_size * 64, font_size * 64);

		hb_shape(font.get_font(), buffer, nullptr, 0);

		unsigned int count;
		hb_glyph_info_t* info = hb_buffer_get_glyph_infos(buffer, &count);
		hb_glyph_position_t* pos = hb_buffer_get_glyph_positions(buffer, &count);
		glyph_indices.resize(count);
		advances.resize(count);
		offsets.resize(count);
		for (unsigned i = 0; i < count; ++i) {
			glyph_indices[i] = static_cast<UINT16>(info[i].codepoint);
			advances[i] = pos[i].x_advance / 64.0f;
			offsets[i].advanceOffset = pos[i].x_offset / 64.0f;
			offsets[i].ascenderOffset = -pos[i].y_offset / 64.0f;
		}

		data.fontFace = hb_directwrite_face_get_font_face(font.get_face());
		data.fontEmSize = font_size;
		data.glyphCount = count;
		data.glyphIndices = glyph_indices.data();
		data.glyphAdvances = advances.data();
		data.glyphOffsets = offsets.data();
		data.isSideways = false;
		data.bidiLevel = 0;

		hb_buffer_destroy(buffer);
	}
};

struct GlyphRunFigure : Figure {
	const GlyphRun& glyph_run;
	Point origin;
	Color color;

	GlyphRunFigure(const GlyphRun& glyph_run, Point origin, Color color) : glyph_run(glyph_run), origin(origin), color(color) {}

	virtual void DrawOn(RenderTarget& target, Point point) const override {
		target.DrawGlyphRunWithColorSupport(
			AsD2DPoint(origin),
			&glyph_run.data,
			nullptr,
			&GetD2DSolidColorBrush(color),
			nullptr
		);
	}
};


struct MainFrameStyle : public TitleBarFrame::Style {
	MainFrameStyle() {
		title.text.assign(L"TextTools");
	}
};


class Canvas : public Placeholder<Assigned, Assigned> {
public:
	Canvas() : font("C:\\Windows\\Fonts\\seguiemj.ttf"), glyph_run(font, 50, "Hello, HarfBuzz! 🤕") {}

protected:
	Size size;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return size = size_ref; }

protected:
	HBFont font;
	GlyphRun glyph_run;
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.add(point_zero, new GlyphRunFigure(glyph_run, Point(0, size.height / 2), Color(Color::Green, 0x80)));
	}
};


int main() {
	global.AddWnd(
		new TitleBarFrame(
			MainFrameStyle(),
			new Canvas()
		)
	);
	global.MessageLoop();
}
