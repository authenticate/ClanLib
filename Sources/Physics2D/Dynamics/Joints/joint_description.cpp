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
**    Arkadiusz Kalinowski
*/

#include "Physics2D/precomp.h"
#include "joint_description.h"

namespace clan
{

JointDescription_Impl::JointDescription_Impl()
{
}

JointDescription::JointDescription()
: impl(new JointDescription_Impl)
{

}
JointDescription::~JointDescription()
{

}

//																											___________________																											
//																											A T T R I B U T E S


void JointDescription::throw_if_null() const
{
	if (!impl)
		throw Exception("JointDescription is null");
}

//																											___________________																											
//																											O P E R A T I O N S

JointDescription &JointDescription::operator =(const JointDescription &copy)
{
	impl = copy.impl;
	return *this;
}

}