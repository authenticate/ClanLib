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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/Components/ribbon_section.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

RibbonSection::RibbonSection(GUIComponent *parent, const std::string &text, int size)
: GUIComponent(parent), text(text), size(size)
{
	set_type_name("ribbon-section");
	func_render().set(this, &RibbonSection::on_render);

	part_section = GUIThemePart(this);
//	font = Font(get_canvas(), "Segoe UI", -11);
	font = Font(get_canvas(), "Tahoma", -11);
}

void RibbonSection::on_render(Canvas &canvas, const Rect &clip_rect)
{
	part_section.render_box(canvas, get_size(), clip_rect);
	Size size_section_text = font.get_text_size(canvas, text);
	font.draw_text(canvas, get_width()/2-size_section_text.width/2, get_height() - 5, text, Colorf::gray40);
}

}