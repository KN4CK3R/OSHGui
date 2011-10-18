#ifndef OSHGUI_EVENT_IEVENT_H_
#define OSHGUI_EVENT_IEVENT_H_

namespace OSHGui
{
	/**
	 * Basisklasse für alle Eventtypen.
	 */
	class IEvent
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