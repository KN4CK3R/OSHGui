#ifndef OSHGUI_EVENT_KEYPRESSEVENTARGS_H_
#define OSHGUI_EVENT_KEYPRESSEVENTARGS_H_

#include "Misc\Strings.h"
#include "KeyboardEvent.h"

namespace OSHGui
{
	/**
	 * Stellt Daten f�r das KeyPressEvent bereit.
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