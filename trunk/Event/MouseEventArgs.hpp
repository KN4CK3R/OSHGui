#ifndef OSHGUI_EVENT_MOUSEEVENTARGS_H_
#define OSHGUI_EVENT_MOUSEEVENTARGS_H_

#include "Drawing\Point.hpp"
#include "MouseEvent.hpp"

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
		MouseEventArgs(MouseEvent *mouseEvent)
		{
			State = mouseEvent->State;
			Position = mouseEvent->Position;
		}
	};
}

#endif