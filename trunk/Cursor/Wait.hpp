#ifndef OSHGUI_CURSOR_WAIT_H_
#define OSHGUI_CURSOR_WAIT_H_

#include "Cursor.hpp"

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