/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_DIRECT3D9VIEWPORTTARGET_HPP
#define OSHGUI_DRAWING_DIRECT3D9VIEWPORTTARGET_HPP

#include "Direct3D9RenderTarget.hpp"
#include "../Rectangle.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Die Direct3D9 Variante des RenderTarget.
		 */
		class OSHGUI_EXPORT Direct3D9ViewportTarget : public Direct3D9RenderTarget<>
		{
		public:
			/**
			 * Erzeugt ein RenderTarget, das den Direct3D9 Viewport f�r Zeichenoperationen verwendet.
			 *
			 * \param owner Renderer Objekt, das das RenderTarget erstellt hat
			 */
			Direct3D9ViewportTarget(Direct3D9Renderer &owner);

			/**
			 * Erzeugt ein RenderTarget, das den angegebenen Bereich f�r Zeichenoperationen verwendet.
			 *
			 * \param owner Renderer Objekt, das das RenderTarget erstellt hat
			 * \param area Bereich f�r Zeichenoperationen
			 */
			Direct3D9ViewportTarget(Direct3D9Renderer &owner, const RectangleF &area);

			virtual bool IsImageryCache() const override;
		};
	}

}

#endif