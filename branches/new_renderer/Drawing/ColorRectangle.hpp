#ifndef OSHGUI_DRAWING_COLORRECTANGLE_HPP
#define OSHGUI_DRAWING_COLORRECTANGLE_HPP

#include "Color.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Describes a rectangle which can have four different colored corners.
		 */
		class OSHGUI_EXPORT ColorRectangle
		{
		public:
			/**
			 * Sets all corners to the same color.
			 *
			 * @param color
			 */
			ColorRectangle(const Color &color);
			/**
			 * Sets both top and botton corners to the same color.
			 *
			 * @param top
			 * @param bottom
			 */
			ColorRectangle(const Color &top, const Color &bottom);
			/**
			 * Sets the color of the corners.
			 *
			 * @param topLeft
			 * @param topRight
			 * @param bottomLeft
			 * @param bottomRight
			 */
			ColorRectangle(const Color &topLeft, const Color &topRight, const Color &bottomLeft, const Color &bottomRight);

			/**
			 * Determinate if the ColorRectangle is monochromatic.
			 *
			 * @return true if all corners have the same color
			 */
			bool IsMonochromatic() const;

			/**
			 * Gets a colored subrectangle.
			 *
			 * @param left left side of this subrectangle (0 - 1)
			 * @param right right side of this subrectangle (0 - 1)
			 * @param top top side of this subrectangle (0 - 1)
			 * @param bottom bottom side of this subrectangle (0 - 1)
			 * @return A ranged ColorRectangle.
			 */
			ColorRectangle GetSubRectangle(float left, float right, float top, float bottom) const;

			/**
			 * Gets the color at a point in the rectangle.
			 *
			 * @param x
			 * @param y
			 * @return The color at the specific point.
			 */
			Color GetColorAtPoint(float x, float y) const;

			ColorRectangle& operator+=(const ColorRectangle &rhs);
			ColorRectangle& operator*=(const ColorRectangle &rhs);
			ColorRectangle& operator*=(float rhs);

			Color TopLeft, TopRight, BottomLeft, BottomRight;
		};

		const ColorRectangle operator+(const ColorRectangle &lhs, const ColorRectangle &rhs);
		const ColorRectangle operator*(const ColorRectangle &lhs, const ColorRectangle &rhs);
		const ColorRectangle operator*(const ColorRectangle &lhs, float rhs);
	}
}

#endif
