#ifndef OSHGUI_CURSOR_WAIT_HPP_
#define OSHGUI_CURSOR_WAIT_HPP_

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