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
		virtual void CreateCursor();
	};
}

#endif