#ifndef __OSHGUI_DRAWING_POINT_H__
#define __OSHGUI_DRAWING_POINT_H__

namespace OSHGui
{
	namespace Drawing
	{
		class Point
		{			
		public:
			Point();
			Point(int X, int Y);
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
			
			void Offset(int X, int Y);
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