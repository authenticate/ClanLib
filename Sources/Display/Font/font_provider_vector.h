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
**    Harry Storbacka
**    Magnus Norddahl
**    Mark Page
*/


#pragma once


#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font.h"
#include <list>
#include <map>

namespace clan
{

class FreetypeFont;
class GlyphPrimitivesArray;
class GlyphPrimitivesArrayOutline;
class GlyphOutline;

class FontEngine_Freetype;

class FontProvider_Vector : public FontProvider
{
/// \name Construction
/// \{

public:

	FontProvider_Vector();

	virtual ~FontProvider_Vector();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns information about the current font.
	virtual FontMetrics get_font_metrics();


/// \}
/// \name Operations
/// \{

public:

	/// \brief Print text on gc.
	virtual void draw_text(Canvas &canvas, float x, float y, const std::string &text, const Colorf &color);

	/// \brief Calculate size of text string.
	virtual Size get_text_size(GraphicContext &gc, const std::string &text);

	int get_glyph_count(const std::string &text);

	GlyphPrimitivesArrayOutline get_glyph_outline(int glyph);

	GlyphPrimitivesArray get_glyph_triangles(int glyph);

	/// \brief Set to draw filled (default)
	void set_filled(bool enable);

	void get_glyphs(
		const std::string &text,
		int *out_glyphs,
		float *out_interspacing_x,
		float *out_interspacing_y);

	void draw_glyphs(
		Canvas &canvas,
		float x,
		float y,
		int *glyphs,
		int length,
		bool filled,
		float *interspacing_x = 0,
		float *interspacing_y = 0,
		const Colorf &color = Colorf::white);

	Sizef get_glyphs_size(int *glyphs, int length);

	int get_character_index(GraphicContext &gc, const std::string &text, const Point &point);

	void load_font(const FontDescription &desc);
	void load_font(const FontDescription &desc, IODevice &file);
	void load_font(const FontDescription &desc, const VirtualDirectory &directory);

/// \}
/// \name Implementation
/// \{

private:

	std::map<int, GlyphOutline*> char_cache;

	FontMetrics metrics;

	FontEngine_Freetype *font_engine;

	int size_height;

	bool is_filled;

/// \}
};

}