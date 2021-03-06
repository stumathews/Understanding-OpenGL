#include "pch.h"
#include "events\Event.h"
#include "events\EventsCommon.h"

namespace gamelib
{
	event::event(event_type type, int event_id): type(type), event_id(event_id)
	{
		// using member init only
	}

	event_type event::get_type()
	{
		return type;
	}

	std::string event::to_str()
	{
		return "generic event";
	}

	std::string operator+(const std::string& str, const event_type type)
	{
		return str + std::to_string(as_integer(type));
	}
}
