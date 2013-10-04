/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_CURSOR_IBEAM_HPP
#define OSHGUI_CURSOR_IBEAM_HPP

#include "Cursor.hpp"

namespace OSHGui
{
	/**
	 * Textauswahl-Cursor
	 */
	class IBeamCursor : public Cursor
	{
	public:
		/**
		 * Konstruktor der Klasse
		 */
		IBeamCursor();
		virtual ~IBeamCursor();
		
	protected:
		virtual void CreateCursor() override;
	};
}

#endif