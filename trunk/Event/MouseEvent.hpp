#ifndef OSHGUI_EVENT_MOUSEEVENT_HPP_
#define OSHGUI_EVENT_MOUSEEVENT_HPP_

#include "Drawing\Point.hpp"
#include "IEvent.hpp"

namespace OSHGui
{
	/**
	 * Mausevent
	 */
	class OSHGUI_EXPORT MouseEvent : public IEvent
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
		MouseEvent() : IEvent(IEvent::Mouse)
		{
			State = Unknown;
			Delta = 0;
		}
		
		MouseEvent(MouseStates State, Drawing::Point Position, int Delta) : IEvent(IEvent::Mouse)
		{
			this->State = State;
			this->Position = Position;
			this->Delta = Delta;
		}
	};
}

#endif