#pragma once
#include "events/Event.h"
#include "Common.h"
#include "GameObject.h"

/// <summary>
/// Details about the direction moved
/// </summary>
enum class Direction {Forward, Back, Left, Right, Undefined};
	
/// <summary>
/// An event that shares details of the object that collided with the player
/// </summary>
class PlayerCollisionEvent : public gamelib::event
{
public:
	explicit PlayerCollisionEvent(std::shared_ptr<GameObject> collider);

	/// <summary>
	/// who the collision was with, ie who collided with the player
	/// </summary>
	std::shared_ptr<GameObject> collider;

	/// <summary>
	/// We can print outself if we need to
	/// </summary>
	/// <returns></returns>
	std::string to_str() override;
};



