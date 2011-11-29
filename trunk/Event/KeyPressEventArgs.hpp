#ifndef OSHGUI_EVENT_KEYPRESSEVENTARGS_HPP_
#define OSHGUI_EVENT_KEYPRESSEVENTARGS_HPP_

#include "Misc\Strings.hpp"
#include "KeyboardMessage.hpp"

namespace OSHGui
{
	/**
	 * Stellt Daten f�r das KeyPressEvent bereit.
	 */
	class OSHGUI_EXPORT KeyPressEventArgs
	{
	public:
		Misc::AnsiChar KeyChar;
		bool Handled;
		
	public:
		KeyPressEventArgs(KeyboardMessage *keyboardEvent)
		{
			KeyChar = keyboardEvent->KeyChar;
			Handled = false;
		}

		KeyPressEventArgs(const KeyboardMessage &keyboardEvent)
		{
			KeyChar = keyboardEvent.KeyChar;
			Handled = false;
		}
	};
}

#endif