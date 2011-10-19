#ifndef OSHGUI_CURSOR_HAND_HPP_
#define OSHGUI_CURSOR_HAND_HPP_

#include "Cursor.hpp"

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