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
**    Mark Page
*/

#pragma once

enum FontClass
{
	font_freetype,
	font_system,
	font_vector,
	font_sprite
};

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<std::string> &args);

private:
	void on_button_clicked_class_freetype(PushButton *button);
	void on_button_clicked_class_system(PushButton *button);
	void on_button_clicked_class_vector(PushButton *button);
	void on_button_clicked_class_sprite(PushButton *button);
	void on_button_clicked_typeface_tahoma(PushButton *button);
	void on_button_clicked_typeface_sans(PushButton *button);
	void on_button_clicked_typeface_ttf(PushButton *button);
	void on_checkbox_state_underline(CheckBox *checkbox);
	void on_checkbox_state_italic(CheckBox *checkbox);
	void on_checkbox_state_subpixel(CheckBox *checkbox);
	void on_checkbox_state_strikeout(CheckBox *checkbox);
	void on_checkbox_state_antialias(CheckBox *checkbox);
	void on_button_clicked_weight_light(PushButton *button);
	void on_button_clicked_weight_normal(PushButton *button);
	void on_button_clicked_weight_bold(PushButton *button);
	void on_button_clicked_size_16(PushButton *button);
	void on_button_clicked_size_32(PushButton *button);
	void on_button_clicked_size_64(PushButton *button);

	void on_input_up(const InputEvent &key);
	void on_window_close();
	void on_lineedit_changed(InputEvent &e);
	void render(DisplayWindow &window);
	void select_font();
	void draw_font_example();
	void draw_font_info();

private:
	bool quit;
	Canvas canvas;
	Window *gui_window_ptr;
	GUIWindowManagerTexture *wm_ptr;
	LineEdit *lineedit_text_ptr;
	PushButton *button_typeface_tahoma_ptr;
	PushButton *button_typeface_sans_ptr;
	std::string font_text;
	FontMetrics font_metrics;
	FontDescription font_desc;
	Size font_size;
	Font small_font;
	Font selected_font;
	FontClass selected_fontclass;
	float last_fps;
	ResourceManager app_resources;
};


