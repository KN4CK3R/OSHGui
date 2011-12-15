#ifndef OSHGUI_CURSOR_IBEAM_HPP
#define OSHGUI_CURSOR_IBEAM_HPP

#include "Cursor.hpp"

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