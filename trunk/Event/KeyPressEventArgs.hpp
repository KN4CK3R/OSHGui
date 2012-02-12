/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EVENT_KEYPRESSEVENTARGS_HPP
#define OSHGUI_EVENT_KEYPRESSEVENTARGS_HPP

#include "Misc/Strings.hpp"
#include "KeyboardMessage.hpp"

namespace OSHGui
{
	/**
	 * Stellt Daten f�r das KeyPressEvent bereit.
	 */
	class OSHGUI_EXPORT KeyPressEventArgs
	{
	public:
		Misc::AnsiChar KeyChar;
		bool Handled;
		
	public:
		/**
		 * Konstruktor der Klasse
		 */
		KeyPressEventArgs(const KeyboardMessage &keyboardEvent)
		{
			KeyChar = keyboardEvent.KeyChar;
			Handled = false;
		}
	};
}

#endif