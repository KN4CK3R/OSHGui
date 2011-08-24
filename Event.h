#ifndef __OSHGUI_EVENT_H__
#define __OSHGUI_EVENT_H__

namespace OSHGui
{
	/**
	 * Basisklasse für alle Eventtypen.
	 */
	class Event
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

		Event(EventTypes Type)
		{
			this->Type = Type;
		}
	};
}

#endif