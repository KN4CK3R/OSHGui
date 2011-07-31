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
		Rectangle::Rectangle(const Rectangle &rect)
		{
			left = rect.left;
			top = rect.top;
			SetWidth(rect.width);
			SetHeight(rect.height);
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
		const int Rectangle::GetLeft()
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
		const int Rectangle::GetTop()
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
		const int Rectangle::GetWidth()
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
		const int Rectangle::GetHeight()
		{
			return height;
		}
		//---------------------------------------------------------------------------
		const int Rectangle::GetRight()
		{
			return right;
		}
		//---------------------------------------------------------------------------
		const int Rectangle::GetBottom()
		{
			return bottom;
		}
		//---------------------------------------------------------------------------
		const Point Rectangle::GetPosition()
		{
			return Point(left, top);
		}
		//---------------------------------------------------------------------------
		const Size Rectangle::GetSize()
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