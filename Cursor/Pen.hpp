#ifndef OSHGUI_CURSOR_PEN_HPP_
#define OSHGUI_CURSOR_PEN_HPP_

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