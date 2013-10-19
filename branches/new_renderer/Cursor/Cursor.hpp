/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_CURSOR_CURSOR_HPP
#define OSHGUI_CURSOR_CURSOR_HPP

#include <memory>
#include "../Exports.hpp"
#include "../Drawing/Point.hpp"
#include "../Drawing/IRenderer.hpp"
#include "../Drawing/ITexture.hpp"
#include "../Drawing/GeometryBuffer.hpp"

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
		virtual void Render(Drawing::IRenderer *renderer, const Drawing::PointF &cursorLocation);

		virtual void Render(const Drawing::PointF &cursorLocation);
		
	protected:
		virtual void CreateCursor();

		Drawing::PointF offset;
		std::shared_ptr<Drawing::ITexture> cursor;

		Drawing::GeometryBufferPtr geometry;
	};
}

#endif