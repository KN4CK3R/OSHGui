#ifndef OSHGUI_CURSOR_HAND_H_
#define OSHGUI_CURSOR_HAND_H_

#include "Cursor.h"

namespace OSHGui
{
	class HandCursor : public Cursor
	{
	public:
		HandCursor();
		virtual ~HandCursor();
		
	private:
		virtual void CreateCursor();
	};
}

#endif