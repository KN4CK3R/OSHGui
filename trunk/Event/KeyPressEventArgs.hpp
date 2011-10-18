#ifndef OSHGUI_EVENT_KEYPRESSEVENTARGS_H_
#define OSHGUI_EVENT_KEYPRESSEVENTARGS_H_

#include "Misc\Strings.hpp"
#include "KeyboardEvent.hpp"

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