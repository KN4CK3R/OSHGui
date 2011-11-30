#include "Rectangle.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Rectangle::Rectangle()
		{
			left = 0;
			top = 0;
			SetWidth(0);
			SetHeight(0);
		}
		//---------------------------------------------------------------------------
		Rectangle::Rectangle(const Size &size)
		{
			left = 0;
			top = 0;
			SetWidth(size.Width);
			SetHeight(size.Height);
		}
		//---------------------------------------------------------------------------
		Rectangle::Rectangle(const Point &location, const Size &size)
		{
			left = location.Left;
			top = location.Top;
			SetWidth(size.Width);
			SetHeight(size.Height);
		}
		//---------------------------------------------------------------------------
		Rectangle::Rectangle(int left, int top, int width, int height)
		{
			this->left = left;
			this->top = top;
			SetWidth(width);
			SetHeight(height);
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		void Rectangle::SetLeft(int left)
		{
			this->left = left;
			right = left + width;
		}
		//---------------------------------------------------------------------------
		int Rectangle::GetLeft() const
		{
			return left;
		}
		//---------------------------------------------------------------------------
		void Rectangle::SetTop(int top)
		{
			this->top = top;
			bottom = top + height;
		}
		//---------------------------------------------------------------------------
		int Rectangle::GetTop() const
		{
			return top;
		}
		//---------------------------------------------------------------------------
		void Rectangle::SetWidth(int width)
		{
			this->width = width;
			right = left + width;
		}
		//---------------------------------------------------------------------------
		int Rectangle::GetWidth() const
		{
			return width;
		}
		//---------------------------------------------------------------------------
		void Rectangle::SetHeight(int height)
		{
			this->height = height;
			bottom = top + height;
		}
		//---------------------------------------------------------------------------
		int Rectangle::GetHeight() const
		{
			return height;
		}
		//---------------------------------------------------------------------------
		int Rectangle::GetRight() const
		{
			return right;
		}
		//---------------------------------------------------------------------------
		int Rectangle::GetBottom() const
		{
			return bottom;
		}
		//---------------------------------------------------------------------------
		Point Rectangle::GetPosition() const
		{
			return Point(left, top);
		}
		//---------------------------------------------------------------------------
		Size Rectangle::GetSize() const
		{
			return Size(width, height);
		}
		//---------------------------------------------------------------------------
		//Operator
		//---------------------------------------------------------------------------
		bool Rectangle::operator == (const Rectangle &equal) const
		{
			return left == equal.left && top == equal.top && width == equal.width && height == equal.height;
		}
		//---------------------------------------------------------------------------
		bool Rectangle::operator != (const Rectangle &equal) const
		{
			return left != equal.left || top != equal.top || width != equal.width || height != equal.height;
		}
		//---------------------------------------------------------------------------
		const Rectangle Rectangle::operator + (const Rectangle& add) const
		{
			return Rectangle(left + add.left, top + add.top, width + add.width, height + add.height);
		}
		//---------------------------------------------------------------------------
		const Rectangle Rectangle::operator + (const Point& add) const
		{
			return Rectangle(left + add.Left, top + add.Top, width, height);
		}
		//---------------------------------------------------------------------------
		const Rectangle Rectangle::operator - (const Rectangle& add) const
		{
			return Rectangle(left - add.left, top - add.top, width - add.width, height - add.height);
		}
		//---------------------------------------------------------------------------
		const Rectangle Rectangle::operator - (const Point& add) const
		{
			return Rectangle(left - add.Left, top - add.Top, width, height);
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void Rectangle::Offset(int left, int top)
		{
			SetLeft(this->left + left);
			SetTop(this->top + top);
		}
		//---------------------------------------------------------------------------
		Rectangle Rectangle::OffsetEx(int left, int top) const
		{
			return Rectangle(this->left + left, this->top + top, width, height);
		}
		//---------------------------------------------------------------------------
		void Rectangle::Inflate(int width, int height)
		{
			SetWidth(this->width + width);
			SetHeight(this->height + height);
		}
		//---------------------------------------------------------------------------
		Rectangle Rectangle::InflateEx(int width, int height) const
		{
			return Rectangle(left, top, this->width + width, this->height + height);
		}
		//---------------------------------------------------------------------------
		bool Rectangle::Contains(const Point &point) const
		{
			return (left <= point.Left && point.Left < right) && (top <= point.Top && point.Top < bottom);
		}
		//---------------------------------------------------------------------------
	}
}