/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_CURSOR_CIRCLE_HPP
#define OSHGUI_CURSOR_CIRCLE_HPP

#include "Cursor.hpp"

namespace OSHGui
{
	/**
	 * Kreis-Cursor
	 */
	class CircleCursor : public Cursor
	{
	public:
		/**
		 * Konstruktor der Klasse
		 */
		CircleCursor();
		virtual ~CircleCursor();
		
	protected:
		virtual void CreateCursor() override;
	};
}

#endif