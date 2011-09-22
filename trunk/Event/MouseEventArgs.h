#ifndef __OSHGUI_MOUSEEVENTARGS_H__
#define __OSHGUI_MOUSEEVENTARGS_H__

#include "Drawing\Point.h"
#include "MouseEvent.h"

namespace OSHGui
{
	/**
	 * Mausevent
	 */
	class MouseEventArgs : public Event
	{
	public:	
		MouseEvent::MouseStates State;
		Drawing::Point Position;
		int Delta;
		bool Handled;
		
	public:
		MouseEvent(MouseEvent::MouseStates State, Drawing::Point Position, int Delta)
		{
			this->State = State;
			this->Position = Position;
			this->Delta = Delta;
			Handled = false;
		}
	};
}

#endif