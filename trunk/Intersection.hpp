#ifndef OSHGUI_INTERSECTION_HPP_
#define OSHGUI_INTERSECTION_HPP_

#include "Drawing/Point.hpp"
#include "Drawing/Size.hpp"

namespace OSHGui
{
	class Intersection
	{
	public:
		static bool TestRectangle(const Drawing::Point &location, const Drawing::Size &size, const Drawing::Point &test)
		{
			int testX = test.X;
			int testY = test.Y;
			int locationX = location.X;
			int locationY = location.Y;

			return ((testX >= locationX && testX <= locationX + size.Width)
				&& (testY >= locationY && testY <= locationY + size.Height));
		}
	};
}

#endif