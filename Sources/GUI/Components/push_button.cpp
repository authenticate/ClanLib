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
**    Kenneth Gangstoe
**    Magnus Norddahl
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/Components/push_button.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/2D/image.h"
#include "API/Display/2D/span_layout.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "../gui_css_strings.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// PushButton_Impl Class:

class PushButton_Impl
{
public:
	PushButton_Impl() : toggle_mode(false), flat_mode(false), icon_position(PushButton::icon_left) {}
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_process_message(GUIMessage &msg);
	void on_style_changed();
	void on_enablemode_changed();
	void create_parts();
	void update_default_state(bool button_focused); // take the state from the message as the focused component hasn't been updated yet at this stage. 

	PushButton *button;
	Callback_v0 func_clicked;
	std::string text;
	Image icon;
	PushButton::IconPosition icon_position;
	bool toggle_mode;
	bool flat_mode;
	GUIThemePart part;
	GUIThemePart part_focus;
};

/////////////////////////////////////////////////////////////////////////////
// PushButton Construction:

PushButton::PushButton(GUIComponent *parent)
: GUIComponent(parent), impl(new PushButton_Impl)
{
	set_type_name(CssStr::PushButton::type_name);
	set_blocks_default_action(true);
	set_focus_policy(focus_local);
	set_double_click_enabled(false);

	func_process_message().set(impl.get(), &PushButton_Impl::on_process_message);
	func_render().set(impl.get(), &PushButton_Impl::on_render);
	func_style_changed().set(impl.get(), &PushButton_Impl::on_style_changed);
	func_enablemode_changed().set(impl.get(), &PushButton_Impl::on_enablemode_changed);

	impl->button = this;
	impl->create_parts();
}

PushButton::~PushButton()
{
}

/////////////////////////////////////////////////////////////////////////////
// PushButton Attributes:

PushButton *PushButton::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	PushButton *object = NULL;
	if (reference_component)
		object = dynamic_cast<PushButton*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find PushButton named item: %1", id));

	return object;
}

bool PushButton::is_pushed() const
{
	return impl->part.get_state(CssStr::pressed);
}

bool PushButton::is_toggle() const
{
	return impl->toggle_mode;
}

bool PushButton::is_flat() const
{
	return impl->flat_mode;
}

Image PushButton::get_icon() const
{
	return impl->icon;
}

PushButton::IconPosition PushButton::get_icon_position() const
{
	return impl->icon_position;
}

const std::string &PushButton::get_text() const
{
	return impl->text;
}

Size PushButton::get_preferred_size() const
{
	return impl->part.get_preferred_size();
}

/////////////////////////////////////////////////////////////////////////////
// PushButton Operations:

void PushButton::set_pushed(bool enable)
{
	if (impl->part.set_state(CssStr::pressed, enable) || 
		impl->part.set_state(CssStr::toggled, enable))
	{
		request_repaint();
	}
}

void PushButton::set_toggle(bool enable)
{
	if (impl->toggle_mode != enable)
	{
		impl->toggle_mode = enable;
		request_repaint();
	}
}

void PushButton::set_flat(bool enable)
{
	if (impl->flat_mode != enable)
	{
		impl->flat_mode = enable;
		request_repaint();
	}
}

void PushButton::set_icon(const Image &icon)
{
	impl->icon = icon;
	request_repaint();
}

void PushButton::set_icon_position(IconPosition pos)
{
	if (impl->icon_position != pos)
	{
		impl->icon_position = pos;
		request_repaint();
	}
}

void PushButton::set_text(const std::string &text)
{
	impl->text = text;
	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// PushButton Events:

Callback_v0 &PushButton::func_clicked()
{
	return impl->func_clicked;
}

/////////////////////////////////////////////////////////////////////////////
// PushButton Implementation:

void PushButton_Impl::create_parts()
{
	part = GUIThemePart(button);
	part.set_state(CssStr::normal, true);
	part.set_state(CssStr::hot, false);
	part.set_state(CssStr::pressed, false);
	part.set_state(CssStr::defaulted, button->is_default());
	part.set_state(CssStr::disabled, !button->is_enabled());
	part.set_state(CssStr::flat, button->is_flat());

	part_focus = GUIThemePart(button, "focus");
}

void PushButton_Impl::on_style_changed()
{
	create_parts();
	update_default_state(false);
}

void PushButton_Impl::on_enablemode_changed()
{
	create_parts();
	button->request_repaint();
}

void PushButton_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = button->get_size();
	part.render_box(canvas, rect, update_rect);

	if (button->has_focus())
	{
		part_focus.render_box(canvas, rect, update_rect);
	}

	Rect content_rect = part.get_content_box(rect);

	if (!icon.is_null())
	{
		Font font = part.get_font();

		GUIThemePartProperty prop_text_color(CssStr::text_color, "black");
		GUIThemePartProperty prop_align(CssStr::align, CssStr::left);
		GUIThemePartProperty prop_vertical_align(CssStr::vertical_align, CssStr::center);

		std::string h_align = part.get_property(prop_align); // left|center|right|justify
		std::string v_align = part.get_property(prop_vertical_align); // top|center|bottom

		Size icon_size = icon.get_size();
		Point center = content_rect.get_center();

		FontMetrics metrics = font.get_font_metrics();
		Colorf text_color = part.get_property(prop_text_color);

		SpanLayout layout;
		if (icon_position == PushButton::icon_top)
		{
			layout.add_image(icon, 0);
			if (!text.empty())
				layout.add_text("\n", font, text_color);
		}
		else if (icon_position == PushButton::icon_left)
		{
			layout.add_image(icon, metrics.get_height() / 2 - metrics.get_ascent() + icon_size.height / 2);
			if (!text.empty())
				layout.add_text(" ", font, text_color);
		}

		layout.add_text(text, font, text_color);

		if (icon_position == PushButton::icon_bottom)
		{
			if (!text.empty())
				layout.add_text("\n", font, text_color);
			layout.add_image(icon, 0);
		}
		else if (icon_position == PushButton::icon_right)
		{
			if (!text.empty())
				layout.add_text(" ", font, text_color);
			layout.add_image(icon, metrics.get_height() / 2 - metrics.get_ascent() + icon_size.height / 2);
		}

		if (h_align == CssStr::left)
			layout.set_align(span_left);
		else if (h_align == CssStr::center)
			layout.set_align(span_center);
		else if (h_align == CssStr::right)
			layout.set_align(span_right);
		else if (h_align == CssStr::justify)
			layout.set_align(span_justify);

		layout.layout(canvas, content_rect.get_width()); // To do: also add support for SpanLayout::layout(canvas, width, height) so we can clip vertically

		if (v_align == CssStr::top)
		{
			layout.set_position(Point(content_rect.left, content_rect.top));
		}
		else if (v_align == CssStr::center)
		{
			layout.set_position(Point(content_rect.left, part.get_vertical_text_align(canvas, font, content_rect).baseline - layout.get_first_baseline_offset() - (layout.get_last_baseline_offset()-layout.get_first_baseline_offset()) / 2));
		}
		else if (v_align == CssStr::bottom)
		{
			layout.set_position(Point(content_rect.left, content_rect.bottom));
		}

		layout.draw_layout_ellipsis(canvas, content_rect);
	}
	else
	{
		part.render_text(canvas, text, content_rect, update_rect);
	}
}

void PushButton_Impl::on_process_message(GUIMessage &msg)
{
	if (!button->is_enabled())
		return;

	if (msg.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input input_msg = msg;
		InputEvent e = input_msg.get_event();

		if (toggle_mode)
		{
			if( e.type == InputEvent::pressed &&
				(e.id == mouse_left || e.id == keycode_return || e.id == keycode_space || e.id == keycode_numpad_enter) )
			{
				part.set_state(CssStr::pressed, true);
				button->request_repaint();
				msg.set_consumed();
			}
			else if( e.type == InputEvent::released &&
				(e.id == mouse_left || e.id == keycode_return || e.id == keycode_space || e.id == keycode_numpad_enter) )
			{
				part.set_state(CssStr::pressed, false);
				part.set_state(CssStr::toggled, !part.get_state("toggled"));
				button->request_repaint();
				msg.set_consumed();
				
				if (!func_clicked.is_null())
					func_clicked.invoke();
			}
		}
		else
		{
			if (e.type == InputEvent::pressed && 
				(e.id == mouse_left || e.id == keycode_return || e.id == keycode_space || e.id == keycode_numpad_enter))
			{
				part.set_state(CssStr::pressed, true);
				button->request_repaint();
				msg.set_consumed();
			}
			else if( e.type == InputEvent::released &&
				(e.id == mouse_left || e.id == keycode_return || e.id == keycode_space || e.id == keycode_numpad_enter) &&
				part.get_state(CssStr::pressed) )
			{
				part.set_state(CssStr::pressed, false);
				button->request_repaint();
				msg.set_consumed();
				
				if (!func_clicked.is_null())
					func_clicked.invoke();
			}
		}
		if (e.type == InputEvent::pressed && (e.id == keycode_left || e.id == keycode_up))
		{
			button->focus_previous();
			msg.set_consumed();
		}
		else if (e.type == InputEvent::pressed && (e.id == keycode_right || e.id == keycode_down))
		{
			button->focus_next();
			msg.set_consumed();
		}
	}
	else if (msg.is_type(GUIMessage_Pointer::get_type_name()))
	{
		GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == GUIMessage_Pointer::pointer_enter)
		{
			part.set_state(CssStr::hot, true);
			button->request_repaint();
			msg.set_consumed();
		}
		else if (pointer.get_pointer_type() == GUIMessage_Pointer::pointer_leave)
		{
			part.set_state(CssStr::hot, false);

			if (!toggle_mode)
				part.set_state(CssStr::pressed, false);
			button->request_repaint();
			msg.set_consumed();
		}
	}
	else if (msg.is_type(GUIMessage_FocusChange::get_type_name()))
	{
		GUIMessage_FocusChange focus_msg = msg;
		if (focus_msg.get_focus_type() == GUIMessage_FocusChange::gained_focus)
		{
			part.set_state(CssStr::focused, true);
			if (!toggle_mode)
				update_default_state(true);
			button->request_repaint();
			msg.set_consumed();
		}
		else 
		{
			part.set_state(CssStr::focused, false);
			if (!toggle_mode)
				update_default_state(false);
			button->request_repaint();
			msg.set_consumed();
		}
	}
}

void PushButton_Impl::update_default_state(bool focus_gained)
{
	bool is_default = false;

	if (focus_gained)
	{
		is_default = true;
	}
	else 
	{
		is_default = button->is_default();

		GUIComponent *focus_comp = button->get_gui_manager().get_focused_component();
		if (focus_comp && (focus_comp != button))
		{
			if (focus_comp->get_blocks_default_action())
				is_default = false;
		}
	}

	part.set_state("defaulted", is_default); 
	button->request_repaint();
}

}