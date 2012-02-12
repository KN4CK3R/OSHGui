/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EVENT_SCROLLEVENTARGS_HPP
#define OSHGUI_EVENT_SCROLLEVENTARGS_HPP

namespace OSHGui
{
	/**
	 * Stellt Daten f�r das ScrollEvent bereit.
	 */
	class OSHGUI_EXPORT ScrollEventArgs
	{
	public:
		int NewValue;
		int OldValue;
		
	public:
		/**
		 * Konstruktor der Klasse
		 *
		 * @param newValue der neue Scroll-Wert
		 * @param oldValue der alte Scroll-Wert
		 */
		ScrollEventArgs(int newValue, int oldValue)
		{
			NewValue = newValue;
			OldValue = oldValue;
		}
	};
}

#endif