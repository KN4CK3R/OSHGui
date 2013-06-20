/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EVENT_MOUSEEVENTARGS_HPP
#define OSHGUI_EVENT_MOUSEEVENTARGS_HPP

#include "../Drawing/Point.hpp"
#include "MouseMessage.hpp"

namespace OSHGui
{
	/**
	 * Stellt Daten f�r MouseEvents bereit.
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