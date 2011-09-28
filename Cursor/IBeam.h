#ifndef __OSHGUI_IBEAMCURSOR_H__
#define __OSHGUI_IBEAMCURSOR_H__

#include "Cursor.h"

namespace OSHGui
{
	class IBeamCursor : public Cursor
	{
	public:
		IBeamCursor();
		virtual ~IBeamCursor();
		
	protected:
		virtual void CreateCursor();
	};
}

#endif