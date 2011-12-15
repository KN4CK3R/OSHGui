#ifndef OSHGUI_CURSOR_WAIT_HPP
#define OSHGUI_CURSOR_WAIT_HPP

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