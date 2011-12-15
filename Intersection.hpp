#ifndef OSHGUI_INTERSECTION_HPP_
#define OSHGUI_INTERSECTION_HPP_

#include "Drawing/Point.hpp"
#include "Drawing/Size.hpp"

namespace OSHGui
{
	class Intersection
	{
	public:
		/**
		 * Prüft, ob sich der Punkt test innerhalb des von location und size aufgespannten Rechtecks liegt.
		 *
		 * @param location
		 * @param size
		 * @param test
		 */
		static bool TestRectangle(const Drawing::Point &location, const Drawing::Size &size, const Drawing::Point &test)
		{
			return (test.X >= location.X && test.X <= location.X + size.Width)
				&& (test.Y >= location.Y && test.Y <= location.Y + size.Height);
		}
	};
}

#endif