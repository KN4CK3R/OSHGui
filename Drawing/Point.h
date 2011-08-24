#ifndef __OSHGUI_DRAWING_POINT_H__
#define __OSHGUI_DRAWING_POINT_H__

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Diese Klasse repräsentiert einen 2D Punkt.
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
			Point(int X, int Y);
			/**
			 * Kopiert einen Punkt.
			 */
			Point(const Point &p);
			
			bool operator == (const Point &equal);
			bool operator != (const Point &equal);
			bool operator < (const Point &greater);
			bool operator <= (const Point &greaterEqual);
			bool operator > (const Point &lower);
			bool operator >= (const Point &lowerEqual);
			Point operator - (const Point &p);
			void operator -= (const Point &p);
			Point operator + (const Point &p);
			void operator += (const Point &p);
			
			/**
			 * Verschiebt den Punkt um X/Y.
			 *
			 * @param X
			 * @param Y
			 */
			void Offset(int X, int Y);
			/**
			 * Kopiert den Punkt und verschiebt ihn um X/Y.
			 *
			 * @param X
			 * @param Y
			 * @return der neue Punkt
			 */
			const Point OffsetEx(int X, int Y);
			
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