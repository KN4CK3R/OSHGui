#ifndef OSHGUI_EVENT_KEYPRESSEVENTARGS_HPP_
#define OSHGUI_EVENT_KEYPRESSEVENTARGS_HPP_

#include "Misc\Strings.hpp"
#include "KeyboardEvent.hpp"

namespace OSHGui
{
	/**
	 * Stellt Daten für das KeyPressEvent bereit.
	 */
	class OSHGUI_EXPORT KeyPressEventArgs
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

		KeyPressEventArgs(const KeyboardEvent &keyboardEvent)
		{
			KeyChar = keyboardEvent.KeyChar;
			Handled = false;
		}
	};
}

#endif