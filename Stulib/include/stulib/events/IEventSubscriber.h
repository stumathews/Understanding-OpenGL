#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Event.h"

namespace gamelib
{
	/// <summary>
	/// An object that can be notified of events needs to make certain functions available so that the event
	/// manager can contact the object through this interface
	/// </summary>
	class IEventSubscriber
	{
		public:
		virtual ~IEventSubscriber();

		/// <summary>
		/// All subscribes must implement a call back function
		/// </summary>
		/// <param name="evt"></param>
		/// <returns></returns>
		virtual std::vector<std::shared_ptr<event>> handle_event(std::shared_ptr<event> evt) = 0;

		/// <summary>
		/// All subscribers must provide a name that identifies them from other subscribers
		/// </summary>
		/// <returns></returns>
		virtual std::string get_subscriber_name() = 0;

		/// <summary>
		/// All subscribers need to have a unique subscriber ID
		/// </summary>
		/// <returns></returns>
		virtual int get_subscriber_id() = 0;
	};
}

