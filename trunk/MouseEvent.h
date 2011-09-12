#ifndef __OSHGUI_MOUSEMESSAGE_H__
#define __OSHGUI_MOUSEMESSAGE_H__

#include "Drawing\Point.h"
#include "Event.h"

namespace OSHGui
{
	/**
	 * Mausevent
	 */
	class MouseEvent : public Event
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
		bool Handled;
		
	public:
		MouseEvent() : Event(Event::Mouse)
		{
			State = Unknown;
			Delta = 0;
			Handled = false;
		}
		MouseEvent(MouseStates State, Drawing::Point Position, int Delta) : Event(Event::Mouse)
		{
			this->State = State;
			this->Position = Position;
			this->Delta = Delta;
			Handled = false;
		}
	};
}

#endif