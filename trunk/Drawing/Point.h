#ifndef __OSHGUI_DRAWING_POINT_H__
#define __OSHGUI_DRAWING_POINT_H__

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Stellt ein geordnetes Paar von x- und y-Koordinaten als ganze Zahlen dar,
		 * das einen Punkt in einem zweidimensionalen Raum definiert.
		 */
		class Point
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
			
			bool operator == (const Point &equal);
			bool operator != (const Point &equal);
			bool operator < (const Point &greater);
			bool operator <= (const Point &greaterEqual);
			bool operator > (const Point &lower);
			bool operator >= (const Point &lowerEqual);
			const Point operator - (const Point &p);
			Point& operator -= (const Point &p);
			Point operator + (const Point &p);
			Point& operator += (const Point &p);
			
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
			const Point OffsetEx(int x, int y);
			
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
	}
}

#endif