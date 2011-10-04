#ifndef OSHGUI_CURSOR_PIPETTE_H_
#define OSHGUI_CURSOR_PIPETTE_H_

#include "Cursor.h"

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