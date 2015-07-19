/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_CURSOR_HAND_HPP
#define OSHGUI_CURSOR_HAND_HPP

#include "Cursor.hpp"

namespace OSHGui
{
	/**
	 * Hand-Cursor
	 */
	class HandCursor : public Cursor
	{
	public:
		/**
		 * Konstruktor der Klasse
		 */
		HandCursor();
		virtual ~HandCursor();
		
	private:
		virtual void CreateCursor() override;
	};
}

#endif