/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2011 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EVENT_KEYEVENTARGS_HPP
#define OSHGUI_EVENT_KEYEVENTARGS_HPP

#include "Misc/Strings.hpp"
#include "KeyboardMessage.hpp"

namespace OSHGui
{
	/**
	 * Stellt Daten für das KeyUpEvent/KeyDownEvent bereit.
	 */
	class OSHGUI_EXPORT KeyEventArgs
	{
	public:
		Key::Keys KeyCode;
		Misc::AnsiChar KeyChar;
		bool Menu,
			 Control,
			 Shift;
		bool Handled;
		
	public:
		/**
		 * Konstruktor der Klasse
		 *
		 * @param keyboardEvent
		 */
		KeyEventArgs(const KeyboardMessage &keyboardEvent)
		{
			KeyCode = keyboardEvent.KeyCode;
			KeyChar = keyboardEvent.KeyChar;
			Menu = keyboardEvent.Menu;
			Control = keyboardEvent.Control;
			Shift = keyboardEvent.Shift;
			Handled = false;
		}
	};
}

#endif