/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
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