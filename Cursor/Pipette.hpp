#ifndef OSHGUI_CURSOR_PIPETTE_HPP_
#define OSHGUI_CURSOR_PIPETTE_HPP_

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