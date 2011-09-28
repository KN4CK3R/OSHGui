#ifndef __OSHGUI_PIPETTECURSOR_H__
#define __OSHGUI_PIPETTECURSOR_H__

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