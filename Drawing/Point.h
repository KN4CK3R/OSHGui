#ifndef __OSHGUI_DRAWING_POINT_H__
#define __OSHGUI_DRAWING_POINT_H__

namespace OSHGui
{
	namespace Drawing
	{
		class Point
		{
		public:
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
			
		public:
			Point() { Point(0, 0); }
			Point(int X, int Y) { this->X = X; this->Y = Y; }
			Point(const Point &p) { X = p.X; Y = p.Y; }
			
			Point operator - (const Point &p) { return Point(X - p.X, Y - p.Y); }
			void operator -= (const Point &p) { X -= p.X; Y -= p.Y; }
			
			void Offset(int X, int Y)
			{
				this->X += X;
				this->Y += Y;
			}
			
			Point OffsetEx(int X, int Y)
			{
				return Point(this->X + X, this->Y + Y);
			}
		};
	}
}

#endif