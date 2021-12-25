#include "pch.h"
#include "PlayerCollisionEvent.h"

using namespace std;

PlayerCollisionEvent::PlayerCollisionEvent(std::shared_ptr<GameObject> collider)
	: event(gamelib::event_type::PlayerCollision)
{
	this->collider = collider;
}

string PlayerCollisionEvent::to_str()
{
	return "PlayerCollisionEvent";
}

