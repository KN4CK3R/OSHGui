#ifndef __OSHGUI_IBEAM_H__
#define __OSHGUI_IBEAM_H__

#include "Cursor.h"

namespace OSHGui
{
	class IBeam : public Cursor
	{
	public:
		IBeam();
		virtual ~IBeam();
	
		virtual void Render(Drawing::IRenderer *renderer, const Drawing::Point &cursorPosition);
		
	protected:
		virtual void CreateCursor();
	};
}

#endif