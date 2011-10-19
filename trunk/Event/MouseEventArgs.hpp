#ifndef OSHGUI_EVENT_MOUSEEVENTARGS_HPP_
#define OSHGUI_EVENT_MOUSEEVENTARGS_HPP_

#include "Drawing\Point.hpp"
#include "MouseEvent.hpp"

namespace OSHGui
{
	/**
	 * Stellt Daten f�r MouseEvents bereit.
	 */
	class OSHGUI_EXPORT MouseEventArgs
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