#ifndef OSHGUI_CURSOR_CURSOR_HPP
#define OSHGUI_CURSOR_CURSOR_HPP

#include <memory>
#include "../Exports.hpp"
#include "../Drawing/Point.hpp"
#include "../Drawing/IRenderer.hpp"
#include "../Drawing/ITexture.hpp"

namespace OSHGui
{
	/**
	 * Standard-Cursor
	 */
	class OSHGUI_EXPORT Cursor
	{
	public:
		/**
		 * Konstruktor der Klasse
		 */
		Cursor();
		virtual ~Cursor();
	
		/**
		 * Zeichnet den Cursor.
		 *
		 * @param renderer
		 * @param cursorLocation
		 */
		virtual void Render(Drawing::IRenderer *renderer, const Drawing::Point &cursorLocation);
		
	protected:
		virtual void CreateCursor();

		Drawing::Point offset;
		std::shared_ptr<Drawing::ITexture> cursor;
	};
}

#endif