#ifndef OSHGUI_CURSOR_CURSOR_H_
#define OSHGUI_CURSOR_CURSOR_H_

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

		Drawing::Point offset;
		std::shared_ptr<Drawing::ITexture> cursor;
	};
}

#endif