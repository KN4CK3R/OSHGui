/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
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
		virtual ~WaitCursor();
		
	protected:
		virtual void CreateCursor() override;
	};
}

#endif