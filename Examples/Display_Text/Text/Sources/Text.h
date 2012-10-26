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

class ExampleText
{
public:
	int start(const std::vector<std::string> &args);
private:
	void on_keyboard_up(const InputEvent &key);
	void on_window_close();

	void update_text(GraphicContext &gc, FrameBuffer &fb_text, Font &font, std::vector<SpanLayout> &layout);
	void draw_text(GraphicContext &gc, Texture &texture, Angle angle);

	bool quit;
	float last_fps;
	static const int text_window_size = 1024;
};