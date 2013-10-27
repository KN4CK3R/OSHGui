/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_CURSOR_WAIT_HPP
#define OSHGUI_CURSOR_WAIT_HPP

#include "Cursor.hpp"

namespace OSHGui
{
	/**
	 * Warte-Cursor
	 */
	class WaitCursor : public Cursor
	{
	public:
		/**
		 * Konstruktor der Klasse
		 */
		WaitCursor();
		
	protected:
		virtual void CreateCursor() override;
	};
}

#endif