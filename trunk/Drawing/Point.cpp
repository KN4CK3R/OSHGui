#include "Point.hpp"

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
		bool Point::operator == (const Point &equal) const
		{
			return X == equal.X && Y == equal.Y;
		}
		//---------------------------------------------------------------------------
		bool Point::operator != (const Point &equal) const
		{
			return X != equal.X || Y != equal.Y;
		}
		//---------------------------------------------------------------------------
		bool Point::operator < (const Point &greater) const
		{
			return X < greater.X && Y < greater.Y;
		}
		//---------------------------------------------------------------------------
		bool Point::operator <= (const Point &greaterEqual) const
		{
			return X <= greaterEqual.X && Y <= greaterEqual.Y;
		}
		//---------------------------------------------------------------------------
		bool Point::operator > (const Point &lower) const
		{
			return X > lower.X && Y > lower.Y;
		}
		//---------------------------------------------------------------------------
		bool Point::operator >= (const Point &lowerEqual) const
		{
			return X >= lowerEqual.X && Y >= lowerEqual.Y;
		}
		//---------------------------------------------------------------------------
		const Point Point::operator - (const Point &p) const
		{
			return OffsetEx(-p.X, -p.Y);
		}
		//---------------------------------------------------------------------------
		Point& Point::operator -= (const Point &p)
		{
			Offset(-p.X, -p.Y);
			return *this;
		}
		//---------------------------------------------------------------------------
		const Point Point::operator + (const Point &p) const
		{
			return OffsetEx(p.X, p.Y);
		}
		//---------------------------------------------------------------------------
		Point& Point::operator += (const Point &p)
		{
			Offset(p.X, p.Y);
			return *this;
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
		const Point Point::OffsetEx(int x, int y) const
		{
			return Point(this->X + x, this->Y + y);
		}
		//---------------------------------------------------------------------------
	}
}