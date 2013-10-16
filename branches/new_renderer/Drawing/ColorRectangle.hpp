#ifndef OSHGUI_DRAWING_COLORRECTANGLE_HPP
#define OSHGUI_DRAWING_COLORRECTANGLE_HPP

#include "Color.hpp"

namespace OSHGui
{
	namespace Drawing
	{
/*!
\brief
	Class that holds details of Colors for the four corners of a rectangle.
*/
		class OSHGUI_EXPORT ColorRectangle
		{
		public:
			/*!
			\brief
				Default constructor
			*/
			ColorRectangle();


			/*!
			\brief
				Constructor for ColorRect objects (via single Color).  Also handles default construction.
			*/
			ColorRectangle(const Color &col);

			ColorRectangle(const Color &top, const Color &bottom);

			/*!
			\brief
				Constructor for ColorRect objects
			*/
			ColorRectangle(const Color &topLeft, const Color &topRight, const Color &bottomLeft, const Color &bottomRight);

			//ColorRect(const ColorRect &other);
			/*!
			\brief
				Set the alpha value to use for all four corners of the ColorRect.

			\param alpha
				Alpha value to use.

			\return
				Nothing.
			*/
			void SetAlpha(std::uint8_t alpha);


			/*!
			\brief
				Set the alpha value to use for the top edge of the ColorRect.

			\param alpha
				Alpha value to use.

			\return
				Nothing.
			*/
			void SetTopAlpha(std::uint8_t alpha);


			/*!
			\brief
				Set the alpha value to use for the bottom edge of the ColorRect.

			\param alpha
				Alpha value to use.

			\return
				Nothing.
			*/
			void SetBottomAlpha(std::uint8_t alpha);


			/*!
			\brief
				Set the alpha value to use for the left edge of the ColorRect.

			\param alpha
				Alpha value to use.

			\return
				Nothing.
			*/
			void SetLeftAlpha(std::uint8_t alpha);


			/*!
			\brief
				Set the alpha value to use for the right edge of the ColorRect.

			\param alpha
				Alpha value to use.

			\return
				Nothing.
			*/
			void SetRightAlpha(std::uint8_t alpha);


			/*!
			\brief
				Determinate the ColorRect is monochromatic or variegated.

			\return
				True if all four corners of the ColorRect has same Color, false otherwise.
			*/
			bool IsMonochromatic() const;


			/*!
			\brief
				Gets a portion of this ColorRect as a subset ColorRect

			\param left
				The left side of this subrectangle (in the range of 0-1 float)
			\param right
				The right side of this subrectangle (in the range of 0-1 float)
			\param top
				The top side of this subrectangle (in the range of 0-1 float)
			\param bottom
				The bottom side of this subrectangle (in the range of 0-1 float)

			\return
				A ColorRect from the specified range
			*/
			ColorRectangle GetSubRectangle(float left, float right, float top, float bottom) const;

			/*!
			\brief
				Get the Color at a point in the rectangle

			\param x
				The x coordinate of the point
			\param y
				The y coordinate of the point

			\return
				The Color at the specified point.
			*/
			Color GetColorAtPoint(float x, float y) const;


			/*!
			\brief
				Set the Color of all four corners simultaneously.

			\param col
				Color that is to be set for all four corners of the ColorRect;
			*/
			void SetColors(const Color &col);


			/*!
			\brief
				Module the alpha components of each corner's Color by a constant.

			\param alpha
				The constant factor to modulate all alpha Color components by.
			*/
			//void modulateAlpha(float alpha);

			/*!
			\brief
				Modulate all components of this Color rect with corresponding components from another Color rect.
			*/
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
