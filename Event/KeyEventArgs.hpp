/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
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
	 * Stellt Daten f�r das KeyUpEvent/KeyDownEvent bereit.
	 */
	class OSHGUI_EXPORT KeyEventArgs
	{
	public:
		/**
		 * Konstruktor der Klasse
		 *
		 * \param keyboardEvent
		 */
		KeyEventArgs(const KeyboardMessage &keyboardEvent)
			: Handled(false),
			  keyData_(keyboardEvent.GetKeyData())
		{
			
		}

		Key GetKeyCode() const
		{
			return keyData_ & Key::KeyCode;
		}

		Key GetModifier() const
		{
			return keyData_ & Key::Modifiers;
		}

		bool IsControl() const
		{
			return (keyData_ & Key::Control) == Key::Control;
		}

		bool IsMenu() const
		{
			return (keyData_ & Key::Alt) == Key::Alt;
		}

		bool IsShift() const
		{
			return (keyData_ & Key::Shift) == Key::Shift;
		}

	public:
		bool Handled;

	private:
		Key keyData_;
	};
}

#endif