#ifndef __OSHGUI_CURSOR_H__
#define __OSHGUI_CURSOR_H__

#include <memory>
#include "../Drawing/Point.h"
#include "../Drawing/IRenderer.h"
#include "../Drawing/ITexture.h"

namespace OSHGui
{
	class Cursor
	{
	public:
		Cursor();
		virtual ~Cursor();
	
		virtual void Render(Drawing::IRenderer *renderer, const Drawing::Point &cursorPosition);
		
	protected:
		virtual void CreateCursor();
	
		std::shared_ptr<Drawing::ITexture> cursor;
	};
}

#endif