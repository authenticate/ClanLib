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
#include "API/Core/Text/logger.h"
#include <algorithm>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Logger Construction:

Logger::Logger()
{
	enable();
}
	
Logger::~Logger()
{
	disable();
}

/////////////////////////////////////////////////////////////////////////////
// Logger Attributes:

std::vector<Logger*> Logger::instances;

Mutex Logger::mutex;

/////////////////////////////////////////////////////////////////////////////
// Logger Operations:

void Logger::enable()
{
	MutexSection mutex_lock(&Logger::mutex);
	if (std::find(instances.begin(), instances.end(), this) == instances.end())
		instances.push_back(this);
}

void Logger::disable()
{
	MutexSection mutex_lock(&Logger::mutex);
	std::vector<Logger*>::iterator il = std::find(instances.begin(), instances.end(), this);
	if(il != instances.end())
		instances.erase(il);
}

void cl_log_event(const std::string &type, const std::string &text)
{
	MutexSection mutex_lock(&Logger::mutex);
	if (Logger::instances.empty())
		return;
	for(std::vector<Logger*>::iterator il = Logger::instances.begin(); il != Logger::instances.end(); il++)
		(*il)->log(type, text);
}

void Logger::log(const std::string &type, const std::string &text)
{
	throw Exception("Implement me");
}

/////////////////////////////////////////////////////////////////////////////
// Logger Implementation:

}