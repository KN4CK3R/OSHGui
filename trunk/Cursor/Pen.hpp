#ifndef OSHGUI_CURSOR_PEN_HPP
#define OSHGUI_CURSOR_PEN_HPP

#include "Cursor.hpp"

namespace OSHGui
{
	class PenCursor : public Cursor
	{
	public:
		PenCursor();
		virtual ~PenCursor();
		
	protected:
		virtual void CreateCursor();
	};
}

#endif