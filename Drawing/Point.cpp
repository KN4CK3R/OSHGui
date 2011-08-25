#include "Point.h"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Point::Point()
		{
			X = 0;
			Y = 0;
		}
		//---------------------------------------------------------------------------
		Point::Point(int x, int y)
		{
			this->X = x;
			this->Y = y;
		}
		//---------------------------------------------------------------------------
		//Operator
		//---------------------------------------------------------------------------
		bool Point::operator == (const Point &equal)
		{
			return X == equal.X && Y == equal.Y;
		}
		//---------------------------------------------------------------------------
		bool Point::operator != (const Point &equal)
		{
			return X != equal.X || Y != equal.Y;
		}
		//---------------------------------------------------------------------------
		bool Point::operator < (const Point &greater)
		{
			return X < greater.X && Y < greater.Y;
		}
		//---------------------------------------------------------------------------
		bool Point::operator <= (const Point &greaterEqual)
		{
			return X <= greaterEqual.X && Y <= greaterEqual.Y;
		}
		//---------------------------------------------------------------------------
		bool Point::operator > (const Point &lower)
		{
			return X > lower.X && Y > lower.Y;
		}
		//---------------------------------------------------------------------------
		bool Point::operator >= (const Point &lowerEqual)
		{
			return X >= lowerEqual.X && Y >= lowerEqual.Y;
		}
		//---------------------------------------------------------------------------
		Point Point::operator - (const Point &p)
		{
			return OffsetEx(-p.X, -p.Y);
		}
		//---------------------------------------------------------------------------
		void Point::operator -= (const Point &p)
		{
			Offset(-p.X, -p.Y);
		}
		//---------------------------------------------------------------------------
		Point Point::operator + (const Point &p)
		{
			return OffsetEx(p.X, p.Y);
		}
		//---------------------------------------------------------------------------
		void Point::operator += (const Point &p)
		{
			Offset(p.X, p.Y);
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void Point::Offset(int x, int y)
		{
			this->X += x;
			this->Y += y;
		}
		//---------------------------------------------------------------------------
		const Point Point::OffsetEx(int x, int y)
		{
			return Point(this->X + x, this->Y + y);
		}
		//---------------------------------------------------------------------------
	}
}