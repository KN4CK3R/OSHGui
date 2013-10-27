/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_CURSOR_PIPETTE_HPP
#define OSHGUI_CURSOR_PIPETTE_HPP

#include "Cursor.hpp"

namespace OSHGui
{
	/**
	 * Pipetten-Cursor
	 */
	class PipetteCursor : public Cursor
	{
	public:
		/**
		 * Konstruktor der Klasse
		 */
		PipetteCursor();
		virtual ~PipetteCursor();
		
	protected:
		virtual void CreateCursor() override;
	};
}

#endif