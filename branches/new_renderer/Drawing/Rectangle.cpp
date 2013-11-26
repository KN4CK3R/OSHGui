/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "Rectangle.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		RectangleF::RectangleF()
			: left(0),
			  top(0)
		{
			SetWidth(0);
			SetHeight(0);
		}
		//---------------------------------------------------------------------------
		RectangleF::RectangleF(const SizeF &size)
			: left(0),
			  top(0)
		{
			SetWidth(size.Width);
			SetHeight(size.Height);
		}
		//---------------------------------------------------------------------------
		RectangleF::RectangleF(const PointF &location, const SizeF &size)
			: left(location.Left),
			  top(location.Top)
		{
			SetWidth(size.Width);
			SetHeight(size.Height);
		}
		//---------------------------------------------------------------------------
		RectangleF::RectangleF(int left, int top, int width, int height)
			: left(left),
			  top(top)
		{
			SetWidth(width);
			SetHeight(height);
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		void RectangleF::SetLeft(int left)
		{
			this->left = left;
			right = left + width;
		}
		//---------------------------------------------------------------------------
		int RectangleF::GetLeft() const
		{
			return left;
		}
		//---------------------------------------------------------------------------
		void RectangleF::SetTop(int top)
		{
			this->top = top;
			bottom = top + height;
		}
		//---------------------------------------------------------------------------
		int RectangleF::GetTop() const
		{
			return top;
		}
		//---------------------------------------------------------------------------
		void RectangleF::SetRight(int right)
		{
			this->right = right;
			width = right - left;
		}
		//---------------------------------------------------------------------------
		int RectangleF::GetRight() const
		{
			return right;
		}
		//---------------------------------------------------------------------------
		void RectangleF::SetWidth(int width)
		{
			this->width = width;
			right = left + width;
		}
		//---------------------------------------------------------------------------
		int RectangleF::GetWidth() const
		{
			return width;
		}
		//---------------------------------------------------------------------------
		void RectangleF::SetHeight(int height)
		{
			this->height = height;
			bottom = top + height;
		}
		//---------------------------------------------------------------------------
		int RectangleF::GetHeight() const
		{
			return height;
		}
		//---------------------------------------------------------------------------
		void RectangleF::SetBottom(int bottom)
		{
			this->bottom = bottom;
			height = bottom - top;
		}
		//---------------------------------------------------------------------------
		int RectangleF::GetBottom() const
		{
			return bottom;
		}
		//---------------------------------------------------------------------------
		PointF RectangleF::GetLocation() const
		{
			return PointF(left, top);
		}
		//---------------------------------------------------------------------------
		SizeF RectangleF::GetSize() const
		{
			return SizeF(width, height);
		}
		//---------------------------------------------------------------------------
		//Operator
		//---------------------------------------------------------------------------
		bool RectangleF::operator == (const RectangleF &equal) const
		{
			return left == equal.left && top == equal.top && width == equal.width && height == equal.height;
		}
		//---------------------------------------------------------------------------
		bool RectangleF::operator != (const RectangleF &equal) const
		{
			return left != equal.left || top != equal.top || width != equal.width || height != equal.height;
		}
		//---------------------------------------------------------------------------
		const RectangleF RectangleF::operator + (const RectangleF& add) const
		{
			return RectangleF(left + add.left, top + add.top, width + add.width, height + add.height);
		}
		//---------------------------------------------------------------------------
		const RectangleF RectangleF::operator + (const PointF& add) const
		{
			return RectangleF(left + add.Left, top + add.Top, width, height);
		}
		//---------------------------------------------------------------------------
		const RectangleF RectangleF::operator - (const RectangleF& add) const
		{
			return RectangleF(left - add.left, top - add.top, width - add.width, height - add.height);
		}
		//---------------------------------------------------------------------------
		const RectangleF RectangleF::operator - (const PointF& add) const
		{
			return RectangleF(left - add.Left, top - add.Top, width, height);
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void RectangleF::Offset(int left, int top)
		{
			SetLeft(this->left + left);
			SetTop(this->top + top);
		}
		//---------------------------------------------------------------------------
		RectangleF RectangleF::OffsetEx(int left, int top) const
		{
			return RectangleF(this->left + left, this->top + top, width, height);
		}
		//---------------------------------------------------------------------------
		void RectangleF::Inflate(int width, int height)
		{
			SetWidth(this->width + width);
			SetHeight(this->height + height);
		}
		//---------------------------------------------------------------------------
		RectangleF RectangleF::InflateEx(int width, int height) const
		{
			return RectangleF(left, top, this->width + width, this->height + height);
		}
		//---------------------------------------------------------------------------
		bool RectangleF::Contains(const PointF &point) const
		{
			return (left <= point.Left && point.Left < right) && (top <= point.Top && point.Top < bottom);
		}
		//---------------------------------------------------------------------------
	}
}