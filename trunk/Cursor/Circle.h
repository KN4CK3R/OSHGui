#ifndef OSHGUI_CURSOR_CIRCLE_H_
#define OSHGUI_CURSOR_CIRCLE_H_

#include "Cursor.h"

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