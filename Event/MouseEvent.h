#ifndef __OSHGUI_MOUSEMESSAGE_H__
#define __OSHGUI_MOUSEMESSAGE_H__

#include "Drawing\Point.h"
#include "IEvent.h"

namespace OSHGui
{
	/**
	 * Mausevent
	 */
	class MouseEvent : public IEvent
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