#ifndef __OSHGUI_WAITCURSOR_H__
#define __OSHGUI_WAITCURSOR_H__

#include "Cursor.h"

namespace OSHGui
{
	class WaitCursor : public Cursor
	{
	public:
		WaitCursor();
		virtual ~WaitCursor();
		
	protected:
		virtual void CreateCursor();
	};
}

#endif