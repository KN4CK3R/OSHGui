#ifndef __OSHGUI_KEYPRESSEVENTARGS_H__
#define __OSHGUI_KEYPRESSEVENTARGS_H__

#include "Misc\Strings.h"
#include "KeyboardEvent.h"

namespace OSHGui
{
	/**
	 * Stellt Daten für das KeyPressEvent bereit.
	 */
	class KeyPressEventArgs
	{
	public:
		Misc::UnicodeChar KeyChar;
		bool Handled;
		
	public:
		KeyPressEventArgs(KeyboardEvent *keyboardEvent)
		{
			KeyChar = keyboardEvent->KeyChar;
			Handled = false;
		}
	};
}

#endif