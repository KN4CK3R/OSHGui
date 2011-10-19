#ifndef OSHGUI_EVENT_IEVENT_H_
#define OSHGUI_EVENT_IEVENT_H_

#include "../Exports.hpp"

namespace OSHGui
{
	/**
	 * Basisklasse für alle Eventtypen.
	 */
	class OSHGUI_EXPORT IEvent
	{
	public:
		enum EventTypes { Mouse, Keyboard };
		enum NextEventTypes
		{
			/**
			 * Event wurde behandelt und soll nicht weitergereicht werden.
			 */
			DontContinue,
			/**
			 * Event wurde nicht behandelt und soll weitergereicht werden.
			 */
			Continue
		};

		EventTypes Type;

		IEvent(EventTypes Type)
		{
			this->Type = Type;
		}
	};
}

#endif