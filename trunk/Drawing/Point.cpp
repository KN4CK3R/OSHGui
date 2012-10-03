/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Point.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Point::Point()
			: X(0), Y(0)
		{

		}
		//---------------------------------------------------------------------------
		Point::Point(int x, int y)
			: X(x), Y(y)
		{

		}
		//---------------------------------------------------------------------------
		//Operator
		//---------------------------------------------------------------------------
		const Point Point::operator-(const Point &p) const
		{
			return OffsetEx(-p.X, -p.Y);
		}
		//---------------------------------------------------------------------------
		Point& Point::operator-=(const Point &p)
		{
			Offset(-p.X, -p.Y);
			return *this;
		}
		//---------------------------------------------------------------------------
		const Point Point::operator+(const Point &p) const
		{
			return OffsetEx(p.X, p.Y);
		}
		//---------------------------------------------------------------------------
		Point& Point::operator+=(const Point &p)
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
		bool operator==(const Point &lhs, const Point &rhs)
		{
			return lhs.Left == rhs.Left && lhs.Top == rhs.Top;
		}
		//---------------------------------------------------------------------------
		bool operator<(const Point &lhs, const Point &rhs)
		{
			return lhs.Left < rhs.Left && lhs.Top < rhs.Top;
		}
		//---------------------------------------------------------------------------
	}
}