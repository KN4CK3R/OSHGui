#ifndef OSHGUI_CURSOR_PIPETTE_HPP
#define OSHGUI_CURSOR_PIPETTE_HPP

#include "Cursor.hpp"

namespace OSHGui
{
	class PipetteCursor : public Cursor
	{
	public:
		PipetteCursor();
		virtual ~PipetteCursor();
		
	protected:
		virtual void CreateCursor();
	};
}

#endif