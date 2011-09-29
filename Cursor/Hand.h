#ifndef __OSHGUI_HANDCURSOR_H__
#define __OSHGUI_HANDCURSOR_H__

#include "Cursor.h"

namespace OSHGui
{
	class HandCursor : public Cursor
	{
	public:
		HandCursor();
		virtual ~HandCursor();
		
	protected:
		virtual void CreateCursor();
	};
}

#endif