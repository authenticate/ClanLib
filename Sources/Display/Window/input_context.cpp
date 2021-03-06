/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "Display/precomp.h"
#include "API/Core/System/exception.h"
#include "API/Display/Window/input_context.h"
#include "input_context_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// InputContext Construction:

InputContext::InputContext()
: impl(new InputContext_Impl)
{
	impl->input_context = impl;
}

InputContext::~InputContext()
{
}

/////////////////////////////////////////////////////////////////////////////
// InputContext Attributes:

int InputContext::get_keyboard_count() const
{
	impl->throw_if_disposed();
	return impl->keyboards.size();
}

int InputContext::get_mouse_count() const
{
	impl->throw_if_disposed();
	return impl->mice.size();
}

int InputContext::get_joystick_count() const
{
	impl->throw_if_disposed();
	return impl->joysticks.size();
}

int InputContext::get_tablet_count() const
{
	impl->throw_if_disposed();
	return impl->tablets.size();
}

InputDevice &InputContext::get_keyboard(int keyboard)
{
	impl->throw_if_disposed();
	return impl->keyboards[keyboard];
}

InputDevice &InputContext::get_mouse(int mouse)
{
	impl->throw_if_disposed();
	return impl->mice[mouse];
}

InputDevice &InputContext::get_joystick(int joystick)
{
	impl->throw_if_disposed();
	return impl->joysticks[joystick];
}

InputDevice &InputContext::get_tablet(int tablet)
{
	impl->throw_if_disposed();
	return impl->tablets[tablet];
}

InputDevice &InputContext::get_device(const std::string& devicename)
{
	impl->throw_if_disposed();
	size_t i;
	for (i = 0; i < impl->joysticks.size(); ++i)
		if (impl->joysticks[i].get_device_name() == devicename)
			return impl->joysticks[i];

	for (i = 0; i < impl->keyboards.size(); ++i)
		if (impl->keyboards[i].get_device_name() == devicename)
			return impl->keyboards[i];

	for (i = 0; i < impl->mice.size(); ++i)
		if (impl->mice[i].get_device_name() == devicename)
			return impl->mice[i];

	for (i = 0; i < impl->tablets.size(); ++i)
		if (impl->tablets[i].get_device_name() == devicename)
			return impl->tablets[i];

	throw Exception("Couldn't find InputDevice named '" + devicename + "'");
}

/////////////////////////////////////////////////////////////////////////////
// InputContext operations:

void InputContext::clear()
{
	impl->clear();
}

void InputContext::add_keyboard(InputDevice keyboard)
{
	impl->add_keyboard(keyboard);
}

void InputContext::add_mouse(InputDevice mouse)
{
	impl->add_mouse(mouse);
}

void InputContext::add_joystick(InputDevice joystick)
{
	impl->add_joystick(joystick);
}

void InputContext::add_tablet(InputDevice tablet)
{
	impl->add_tablet(tablet);
}

void InputContext::process_messages()
{
	impl->process_messages();
}

void InputContext::dispose()
{
	impl->dispose();
}

bool InputContext::is_disposed() const
{
	return impl->is_disposed();
}

/////////////////////////////////////////////////////////////////////////////
// InputContext implementation:

}
