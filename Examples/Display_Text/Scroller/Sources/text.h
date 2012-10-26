/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

class Text
{
public:
	Text();
	void init(GraphicContext &gc, const FontDescription &font_description, const DomElement &configuration, const char *text_name);
	void draw(GraphicContext &gc, const Rect &rect);
	void run(int time_delta_ms, int speed);
private:
	void init_stage1_count_glyphs(UTF8_Reader &reader);
	void init_stage2_examine_glyphs(GraphicContext &gc, UTF8_Reader &reader);
	std::string build_text(GraphicContext &gc, int width, int xoffset, int &out_draw_offset);

private:
	Font font;
	std::string text;
	FontMetrics font_metrics;

	std::vector<int> glyph_offset;
	std::vector<int> glyph_width;

	int total_width;

	int scroller_xoffset;

	int time_counter;
};