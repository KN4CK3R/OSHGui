#ifndef __OSHGUI_MOUSEMESSAGE_H__
#define __OSHGUI_MOUSEMESSAGE_H__

#include "Drawing\Point.h"

namespace OSHGui
{
	class MouseEvent : public Event
	{
	public:
		enum MouseStates { None, Move, LeftDown, LeftUp, RightDown, RightUp, Scroll };
	
		MouseStates State;
		Drawing::Point Position;
		int Delta;
		bool Handled;
		
	public:
		MouseEvent() : Event(Event::Mouse)
		{
			State = None;
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