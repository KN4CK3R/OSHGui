#ifndef OSHGUI_CURSOR_CIRCLE_HPP_
#define OSHGUI_CURSOR_CIRCLE_HPP_

#include "Cursor.hpp"

namespace OSHGui
{
	class CircleCursor : public Cursor
	{
	public:
		CircleCursor();
		virtual ~CircleCursor();
		
	protected:
		virtual void CreateCursor();
	};
}

#endif