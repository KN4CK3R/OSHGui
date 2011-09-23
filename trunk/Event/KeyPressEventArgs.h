#ifndef __OSHGUI_KEYPRESSEVENTARGS_H__
#define __OSHGUI_KEYPRESSEVENTARGS_H__

#include "Misc\String.h"

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
		KeyPressEventArgs(Misc::UnicodeChar keyChar)
		{
			KeyChar = keyChar;
			Handled = false;
		}
	};
}

#endif