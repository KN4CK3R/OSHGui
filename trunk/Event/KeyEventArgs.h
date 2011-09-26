#ifndef __OSHGUI_KEYEVENTARGS_H__
#define __OSHGUI_KEYEVENTARGS_H__

#include "Misc\Strings.h"
#include "KeyboardEvent.h"

namespace OSHGui
{
	/**
	 * Stellt Daten f�r das KeyUpEvent/KeyDownEvent bereit.
	 */
	class KeyEventArgs
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