#ifndef OSHGUI_EVENT_IEVENT_HPP_
#define OSHGUI_EVENT_IEVENT_HPP_

#include "../Exports.hpp"

namespace OSHGui
{
	/**
	 * Basisklasse für alle Eventtypen.
	 */
	class OSHGUI_EXPORT IEvent
	{
	public:
		enum EventTypes
		{
			/**
			 * Maus Event
			 */
			Mouse,
			/**
			 * Tastatur Event
			 */
			Keyboard
		};

		EventTypes Type;

		IEvent(EventTypes Type)
		{
			this->Type = Type;
		}
	};
}

#endif