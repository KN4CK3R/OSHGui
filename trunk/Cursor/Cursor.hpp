#ifndef OSHGUI_CURSOR_CURSOR_H_
#define OSHGUI_CURSOR_CURSOR_H_

#include <memory>
#include "../Exports.hpp"
#include "../Drawing/Point.hpp"
#include "../Drawing/IRenderer.hpp"
#include "../Drawing/ITexture.hpp"

namespace OSHGui
{
	class OSHGUI_EXPORT Cursor
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