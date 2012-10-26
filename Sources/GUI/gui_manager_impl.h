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
**    Mark Page
*/

#pragma once

#include "API/Core/Signals/callback_1.h"
#include "API/Core/Signals/callback_v2.h"
#include "API/Core/CSS/css_document.h"
#include "API/Core/CSS/css_property.h"
#include "API/Core/System/timer.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_description.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/accelerator_table.h"
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/gui_theme.h"
#include "gui_font_cache.h"
#include "Theme/gui_theme_layout_manager.h"
#include <vector>
#include <map>

namespace clan
{

class GUIMessage;
class GUITheme;
class GUIWindowManager;
class GUIComponent_Impl;
class InputEvent;
class Font;
class GUITopLevelWindow_Alive;

class GUITopLevelWindow
{
public:
	GUITopLevelWindow() : alive(new int) {}

	GUIComponent *component;
	GUIComponent *focused_component;
	GUIComponent *owner;
	GUIComponent *proximity_component;
	std::vector<Rect> update_regions;

private:
	std::shared_ptr<int> alive;	// Shared Pointer, used to determine if this class is active
	friend class GUITopLevelWindow_Alive;
};

/// Helper class, used to determine when a top level window has been destroyed
class GUITopLevelWindow_Alive
{
/// \name Construction
/// \{
public:
	/// \brief Creates a alive object
	///
	/// \param component = The component (NULL or a valid component)
	GUITopLevelWindow_Alive(GUITopLevelWindow *window);

public:
	/// \brief Determines if the component is destroyed
	///
	/// \return true if destroyed
	bool is_null() const;

private:
	std::weak_ptr<int> window_alive;
};

class GUIManager_Impl
{
/// \name Construction
/// \{
public:
	GUIManager_Impl();
	~GUIManager_Impl();

/// \}
/// \name Attributes
/// \{
public:
	GUIComponent *get_focus_component();
	GUIComponent *get_owner_component(const GUIComponent *component);
	const GUIComponent *get_owner_component(const GUIComponent *component) const;
	bool has_focus(const GUIComponent *component) const;
	GUITopLevelWindow *get_toplevel_window(const GUIComponent * const component) const;

	/// \brief Search children for component with is_cancel() set.
	GUIComponent *get_cancel_component(GUIComponent *comp=0);

	/// \brief Search children for component with is_default() set.
	GUIComponent *get_default_component(GUIComponent *comp=0);

	Font get_registered_font(const FontDescription &desc);

	std::vector<GUITopLevelWindow *> root_components;
	Signal_v1<GUIMessage &> sig_filter_message;
	Callback_0<int> func_exec_handler;
	CSSDocument css_document;
	GUIThemeLayoutManager layout_manager;

	GUIComponent *mouse_capture_component;
	GUIComponent *mouse_over_component;
	GUITheme theme;
	GUIWindowManager window_manager;
	bool exit_flag;
	int exit_code;
	AcceleratorTable accel_table;
	GUIWindowManagerSite wm_site;
	Callback_v1<GUITopLevelWindow *> func_focus_lost;
	Callback_v1<GUITopLevelWindow *> func_focus_gained;
	Callback_v2<GUITopLevelWindow *, const Size &> func_resize;
	Callback_v2<GUITopLevelWindow *, const Rect &> func_paint;
	Callback_v1<GUITopLevelWindow *> func_close;
	Callback_v1<GUITopLevelWindow *> func_destroy;
	Callback_v2<GUITopLevelWindow *, const InputEvent &> func_input_received;
	GUIFontCache font_cache;

/// \}
/// \name Operations
/// \{
public:
	void add_component(GUIComponent *component, GUIComponent *owner, GUITopLevelDescription desc);
	void remove_component(GUIComponent_Impl *component_impl);
	void gain_focus(GUIComponent *component);
	void loose_focus(GUIComponent *component);
	void set_enabled(GUIComponent *component, bool enable);
	void dispatch_message(GUIMessage &message);

	std::vector<CSSProperty> &get_properties(const std::string &element_name) const;
	void reset_properties();

	void register_font(const Font &font, const FontDescription &desc);

	bool is_constant_repaint_enabled() const;
	bool is_constant_repaint_enabled(GUIComponent *component) const;
	void invalidate_constant_repaint_components();

	void process_standard_gui_keys(GUIMessage &message);

/// \}
/// \name Implementation
/// \{
private:
	void deliver_message(GUIMessage &message);

	void on_focus_lost(GUITopLevelWindow *toplevel_window);
	void on_focus_gained(GUITopLevelWindow *toplevel_window);
	void on_resize(GUITopLevelWindow *toplevel_window, const Size &new_size);
	void on_paint(GUITopLevelWindow *toplevel_window, const Rect &update_rect);
	void on_close(GUITopLevelWindow *toplevel_window);
	void on_destroy(GUITopLevelWindow *toplevel_window);
	void on_input_received(GUITopLevelWindow *toplevel_window, const InputEvent &input_event);
	mutable std::map< std::string, std::vector<CSSProperty> > properties_cache;

	struct NamedFontCacheEntry
	{
		Font font;
		FontDescription desc;
	};

	std::vector<NamedFontCacheEntry> named_font_cache;

/// \}
};

}