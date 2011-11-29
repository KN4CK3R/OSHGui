#ifndef OSHGUI_EVENT_KEYEVENTARGS_HPP_
#define OSHGUI_EVENT_KEYEVENTARGS_HPP_

#include "Misc\Strings.hpp"
#include "KeyboardMessage.hpp"

namespace OSHGui
{
	/**
	 * Stellt Daten f�r das KeyUpEvent/KeyDownEvent bereit.
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
		KeyEventArgs(KeyboardMessage *keyboardEvent)
		{
			KeyCode = keyboardEvent->KeyCode;
			KeyChar = keyboardEvent->KeyChar;
			Menu = keyboardEvent->Menu;
			Control = keyboardEvent->Control;
			Shift = keyboardEvent->Shift;
			Handled = false;
		}

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