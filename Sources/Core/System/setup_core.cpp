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
*/

#include "Core/precomp.h"
#include "API/Core/System/setup_core.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/thread_local_storage.h"
#include "API/Core/System/system.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SetupCore Construction:

SetupCore::SetupCore()
{
	ThreadLocalStorage::create_initial_instance();
#ifndef DISABLE_SSE2
	if (!System::detect_cpu_extension(System::sse2))
	{
		throw Exception("Sorry, ClanLib 2.4 and higher requires a processor capable of SSE2 instructions. (Update your CPU)");
	}
#endif
}

SetupCore::~SetupCore()
{
}

/////////////////////////////////////////////////////////////////////////////
// SetupCore Implementation:

}