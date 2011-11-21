#ifndef OSHGUI_EVENT_MOUSEEVENTARGS_HPP_
#define OSHGUI_EVENT_MOUSEEVENTARGS_HPP_

#include "Drawing\Point.hpp"
#include "MouseMessage.hpp"

namespace OSHGui
{
	/**
	 * Stellt Daten für MouseEvents bereit.
	 */
	class OSHGUI_EXPORT MouseEventArgs
	{
	public:
		MouseMessage::MouseStates State;
		Drawing::Point Position;
		
	public:
		MouseEventArgs(MouseMessage *mouseEvent)
		{
			State = mouseEvent->State;
			Position = mouseEvent->Position;
		}

		MouseEventArgs(const MouseMessage &mouseEvent)
		{
			State = mouseEvent.State;
			Position = mouseEvent.Position;
		}
	};
}

#endif