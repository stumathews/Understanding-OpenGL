#pragma once

#include <memory>

#include "Event.h"
#include "../GameObject.h"

namespace gamelib
{
	/// <summary>
	/// Types of 'simple' events that dont pass information along with the event itself 
	/// </summary>
	enum class GameObjectEventContext
	{
		// Change view 
		FirstPersonView, TopView, ThirdPersonView, 	LeftSideView, RightSideView,

		// Movement modes
		UseCatmulRom, FreePlayerWithCamera, FreeView,	

		// Levels
		Level1, Level2, Level3,
		ChangeVehicle,

		// Speed commands
		IncreaseSpeed,
		DecreaseSpeed,

		// Testing
		TestEvent,

		// Lighting commands
		MainSceneLighting,
		Spotlight1Lighting,
		Spotlight2Lighting,		
		RealisticLighting,
		UseFog,

		// Movement commands
		MoveUp,
		MoveDown, 
		MoveLeft,
		MoveRight,

		// Options
		DetachCamera,
		MultiPass
	};

	/// <summary>
	/// Game Object event is a generic event that can be raised that only contains a simple payload of the context or nature of the event
	/// without specifying any additinoal information. This is therefore a flexible and generic event
	/// </summary>
	class GameObjectEvent : public event
	{
	public:
		GameObjectEvent(const int event_id, GameObject *gameObject, GameObjectEventContext context ) 
			: event(event_type::GameObject, event_id), 
			context(context), 
			game_object(std::move(gameObject))
		{
		}

		// So what is this event all about anyway?
		GameObjectEventContext context;

		// Is it related to a specific game object?
		GameObject *game_object;

		// Allow the handler of the event to understand what the derived event is from its base class
		event_type get_type() override;
		std::string to_str() override;
		
	};
}

