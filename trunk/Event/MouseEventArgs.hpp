#ifndef OSHGUI_EVENT_MOUSEEVENTARGS_HPP
#define OSHGUI_EVENT_MOUSEEVENTARGS_HPP

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
		Drawing::Point Location;
		
	public:
		/**
		 * Konstruktor der Klasse
		 */
		MouseEventArgs(const MouseMessage &mouseEvent)
		{
			State = mouseEvent.State;
			Location = mouseEvent.Location;
		}
	};
}

#endif