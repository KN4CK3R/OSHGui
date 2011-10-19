#ifndef OSHGUI_EVENT_KEYEVENTARGS_HPP_
#define OSHGUI_EVENT_KEYEVENTARGS_HPP_

#include "Misc\Strings.hpp"
#include "KeyboardEvent.hpp"

namespace OSHGui
{
	/**
	 * Stellt Daten für das KeyUpEvent/KeyDownEvent bereit.
	 */
	class OSHGUI_EXPORT KeyEventArgs
	{
	public:
		Key::Keys KeyCode;
		Misc::UnicodeChar KeyChar;
		bool Menu,
			 Control,
			 Shift;
		bool Handled;
		
	public:
		KeyEventArgs(KeyboardEvent *keyboardEvent)
		{
			KeyCode = keyboardEvent->KeyCode;
			KeyChar = keyboardEvent->KeyChar;
			Menu = keyboardEvent->Menu;
			Control = keyboardEvent->Control;
			Shift = keyboardEvent->Shift;
			Handled = false;
		}
	};
}

#endif