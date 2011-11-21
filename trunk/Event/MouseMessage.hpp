#ifndef OSHGUI_EVENT_MOUSEMESSAGE_HPP_
#define OSHGUI_EVENT_MOUSEMESSAGE_HPP_

#include "Drawing\Point.hpp"
#include "IEvent.hpp"

namespace OSHGui
{
	/**
	 * Mausevent
	 */
	class OSHGUI_EXPORT MouseMessage : public IEvent
	{
	public:
		enum MouseStates
		{
			/**
			 * Unbekannt
			 */
			Unknown,
			/**
			 * Maus wurde bewegt.
			 */
			Move,
			/**
			 * Linke Maustaste wurde gedrückt.
			 */
			LeftDown,
			/**
			 * Linke Maustaste wurde losgelassen.
			 */
			LeftUp,
			/**
			 * Rechte Maustaste wurde gedrückt.
			 */
			RightDown,
			/**
			 * Rechte Maustaste wurde losgelassen.
			 */
			RightUp,
			/**
			 * Mausrad wurde gedreht.
			 */
			Scroll
		};
	
		MouseStates State;
		Drawing::Point Position;
		int Delta;
		
	public:
		MouseMessage() : IEvent(IEvent::Mouse)
		{
			State = Unknown;
			Delta = 0;
		}
		
		MouseMessage(MouseStates State, Drawing::Point Position, int Delta) : IEvent(IEvent::Mouse)
		{
			this->State = State;
			this->Position = Position;
			this->Delta = Delta;
		}
	};
}

#endif