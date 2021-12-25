#pragma once
//#include "Common.h"
//#include <memory>
//#include "events\Event.h"
//#include "PositionChangeEvent.h"
//
//class player_moved_event final : public gamelib::event
//{
//public:
//	explicit player_moved_event(std::shared_ptr<int> &component, glm::vec3 position) :
//		event(gamelib::event_type::PlayerMovedEventType), position(position), player(component)
//	{
//	}
//
//	std::shared_ptr<int> get_player_component() const { return player; }
//	std::string to_str() override;
//	glm::vec3 position;
//private:
//	std::shared_ptr<int> player;
//};
//
//
