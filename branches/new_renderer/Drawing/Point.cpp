/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
		PointF::PointF()
			: X(0), Y(0)
		{

		}
		//---------------------------------------------------------------------------
		PointF::PointF(int x, int y)
			: X(x), Y(y)
		{

		}
		//---------------------------------------------------------------------------
		//Operator
		//---------------------------------------------------------------------------
		const PointF PointF::operator-(const PointF &p) const
		{
			return OffsetEx(-p.X, -p.Y);
		}
		//---------------------------------------------------------------------------
		PointF& PointF::operator-=(const PointF &p)
		{
			Offset(-p.X, -p.Y);
			return *this;
		}
		//---------------------------------------------------------------------------
		const PointF PointF::operator+(const PointF &p) const
		{
			return OffsetEx(p.X, p.Y);
		}
		//---------------------------------------------------------------------------
		PointF& PointF::operator+=(const PointF &p)
		{
			Offset(p.X, p.Y);
			return *this;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void PointF::Offset(int x, int y)
		{
			this->X += x;
			this->Y += y;
		}
		//---------------------------------------------------------------------------
		const PointF PointF::OffsetEx(int x, int y) const
		{
			return PointF(this->X + x, this->Y + y);
		}
		//---------------------------------------------------------------------------
		bool operator==(const PointF &lhs, const PointF &rhs)
		{
			return lhs.Left == rhs.Left && lhs.Top == rhs.Top;
		}
		//---------------------------------------------------------------------------
		bool operator<(const PointF &lhs, const PointF &rhs)
		{
			return lhs.Left < rhs.Left && lhs.Top < rhs.Top;
		}
		//---------------------------------------------------------------------------
	}
}