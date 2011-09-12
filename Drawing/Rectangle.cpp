#include "Rectangle.h"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Rectangle::Rectangle()
		{
			this->left = 0;
			this->top = 0;
			SetWidth(0);
			SetHeight(0);
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
		int Rectangle::GetLeft()
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
		int Rectangle::GetTop()
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
		int Rectangle::GetWidth()
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
		int Rectangle::GetHeight()
		{
			return height;
		}
		//---------------------------------------------------------------------------
		int Rectangle::GetRight()
		{
			return right;
		}
		//---------------------------------------------------------------------------
		int Rectangle::GetBottom()
		{
			return bottom;
		}
		//---------------------------------------------------------------------------
		Point Rectangle::GetPosition()
		{
			return Point(left, top);
		}
		//---------------------------------------------------------------------------
		Size Rectangle::GetSize()
		{
			return Size(width, height);
		}
		//---------------------------------------------------------------------------
		//Operator
		//---------------------------------------------------------------------------
		bool Rectangle::operator == (const Rectangle &equal)
		{
			return left == equal.left && top == equal.top && width == equal.width && height == equal.height;
		}
		//---------------------------------------------------------------------------
		bool Rectangle::operator != (const Rectangle &equal)
		{
			return left != equal.left || top != equal.top || width != equal.width || height != equal.height;
		}
		//---------------------------------------------------------------------------
		Rectangle Rectangle::operator + (const Rectangle& add) const
		{
			return Rectangle(left + add.left, top + add.top, width + add.width, height + add.height);
		}
		//---------------------------------------------------------------------------
		Rectangle Rectangle::operator + (const Point& add) const
		{
			return Rectangle(left + add.Left, top + add.Top, width, height);
		}
		//---------------------------------------------------------------------------
		Rectangle Rectangle::operator - (const Rectangle& add) const
		{
			return Rectangle(left - add.left, top - add.top, width - add.width, height - add.height);
		}
		//---------------------------------------------------------------------------
		Rectangle Rectangle::operator - (const Point& add) const
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
		Rectangle Rectangle::OffsetEx(int left, int top)
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
		Rectangle Rectangle::InflateEx(int width, int height)
		{
			return Rectangle(left, top, this->width + width, this->height + height);
		}
		//---------------------------------------------------------------------------
		bool Rectangle::Contains(const Point &point)
		{
			return (left <= point.Left && point.Left <= right) && (top <= point.Top && point.Top <= bottom);
		}
		//---------------------------------------------------------------------------
	}
}