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
		virtual void CreateCursor();
	};
}

#endif