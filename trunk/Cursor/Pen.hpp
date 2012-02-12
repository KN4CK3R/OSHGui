/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_CURSOR_PEN_HPP
#define OSHGUI_CURSOR_PEN_HPP

#include "Cursor.hpp"

namespace OSHGui
{
	/**
	 * Stift-Cursor
	 */
	class PenCursor : public Cursor
	{
	public:
		/**
		 * Konstruktor der Klasse
		 */
		PenCursor();
		virtual ~PenCursor();
		
	protected:
		virtual void CreateCursor();
	};
}

#endif