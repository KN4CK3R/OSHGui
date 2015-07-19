/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_POINT_HPP
#define OSHGUI_DRAWING_POINT_HPP

#include "../Exports.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Stellt ein geordnetes Paar von x- und y-Koordinaten als ganze Zahlen dar,
		 * das einen Punkt in einem zweidimensionalen Raum definiert.
		 */
		class OSHGUI_EXPORT Point
		{
		public:
			/**
			 * Erstellt einen Punkt mit den Koordinaten 0/0.
			 */
			Point();
			/**
			 * Erstellt einen Punkt mit den Koordinaten X/Y.
			 */
			Point(int x, int y);
			
			friend bool operator==(const Point &lhs, const Point &rhs);
			friend bool operator<(const Point &lhs, const Point &rhs);

			const Point operator-(const Point &p) const;
			Point& operator-=(const Point &p);
			const Point operator+(const Point &p) const;
			Point& operator+=(const Point &p);
			
			/**
			 * Verschiebt den Punkt um X/Y.
			 *
			 * @param x
			 * @param y
			 */
			void Offset(int x, int y);
			/**
			 * Kopiert den Punkt und verschiebt ihn um X/Y.
			 *
			 * @param x
			 * @param y
			 * @return der neue Punkt
			 */
			const Point OffsetEx(int x, int y) const;
			
			union
			{
				int X;
				int Left;
			};
			union
			{
				int Y;
				int Top;
			};
		};

		bool operator==(const Point &lhs, const Point &rhs);
		inline bool operator!=(const Point &lhs, const Point &rhs) { return !(lhs == rhs); }
		bool operator<(const Point &lhs, const Point &rhs);
		inline bool operator>(const Point &lhs, const Point &rhs) { return rhs < lhs; }
		inline bool operator>=(const Point &lhs, const Point &rhs) { return !(rhs < lhs); }
		inline bool operator<=(const Point &lhs, const Point &rhs) { return !(rhs > lhs); }
	}
}

#endif