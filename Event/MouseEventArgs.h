#ifndef __OSHGUI_MOUSEEVENTARGS_H__
#define __OSHGUI_MOUSEEVENTARGS_H__

#include "Drawing\Point.h"
#include "MouseEvent.h"

namespace OSHGui
{
	/**
	 * Stellt Daten für MouseEvents bereit.
	 */
	class MouseEventArgs
	{
	public:	
		MouseEvent::MouseStates State;
		Drawing::Point Position;
		
	public:
		MouseEventArgs(MouseEvent::MouseStates State, Drawing::Point Position)
		{
			this->State = State;
			this->Position = Position;
		}
	};
}

#endif