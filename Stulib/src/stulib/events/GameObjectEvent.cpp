#include "pch.h"
#include "events\GameObjectEvent.h"

// Allow the handler of the event to understand what the derived event is from its base class

gamelib::event_type gamelib::GameObjectEvent::get_type() 
{
	return event_type::GameObject;
}

std::string gamelib::GameObjectEvent::to_str() 
{ 
	return "GameObjectEvent"; 
}
