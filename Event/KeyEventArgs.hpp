/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EVENT_KEYEVENTARGS_HPP
#define OSHGUI_EVENT_KEYEVENTARGS_HPP

#include "../Misc/Strings.hpp"
#include "KeyboardMessage.hpp"

namespace OSHGui
{
	/**
	 * Stellt Daten für das KeyUpEvent/KeyDownEvent bereit.
	 */
	class OSHGUI_EXPORT KeyEventArgs
	{
	public:
		/**
		 * Konstruktor der Klasse
		 *
		 * @param keyboardEvent
		 */
		KeyEventArgs(const KeyboardMessage &keyboardEvent)
			: keyData(keyboardEvent.GetKeyData()),
			  Handled(false)
		{
			
		}

		Key::Keys GetKeyCode() const
		{
			return keyData & Key::KeyCode;
		}

		Key::Keys GetModifier() const
		{
			return keyData & Key::Modifiers;
		}

		bool IsControl() const
		{
			return (keyData & Key::Control) == Key::Control;
		}

		bool IsMenu() const
		{
			return (keyData & Key::Alt) == Key::Alt;
		}

		bool IsShift() const
		{
			return (keyData & Key::Shift) == Key::Shift;
		}

	public:
		bool Handled;

	private:
		Key::Keys keyData;
	};
}

#endif