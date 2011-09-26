#ifndef __OSHGUI_KEYEVENTARGS_H__
#define __OSHGUI_KEYEVENTARGS_H__

#include "Misc\String.h"
#include "KeyBoardEvent."

namespace OSHGui
{
	/**
	 * Stellt Daten für das KeyPressEvent bereit.
	 */
	class KeyPressEventArgs
	{
	public:
		Key::Keys KeyCode;
		Misc::UnicodeChar KeyChar;
		bool Menu,
			 Control,
			 Shift;
		bool Handled;
		
	public:
		KeyPressEventArgs(KeyboardEvent *keyboardEvent)
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