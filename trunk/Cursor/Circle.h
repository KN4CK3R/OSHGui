#ifndef __OSHGUI_CIRCLECURSOR_H__
#define __OSHGUI_CIRCLECURSOR_H__

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