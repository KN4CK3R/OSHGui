#ifndef __OSHGUI_DRAWING_RECTANGLE_H__
#define __OSHGUI_DRAWING_RECTANGLE_H__

#include "Point.h"
#include "Size.h"

namespace OSHGui
{
	namespace Drawing
	{
		class Rectangle
		{
		private:
			int left,
				top,
				width,
				height;
			int bottom,
				right;
				
		public:
			void SetLeft(int left) { this->left = left; right = left + width; }
			const int GetLeft() { return left; }
			void SetTop(int top) { this->top = top; bottom = top + height; }
			const int GetTop() { return top; }
			void SetWidth(int width) { this->width = width; right = left + width; }
			const int GetWidth() { return width; }
			void SetHeight(int height) { this->height = height; bottom = top + height; }
			const int GetHeight() { return height; }
			const int GetRight() { return right; }
			const int GetBottom() { return bottom; }
			
		public:
			Rectangle() { Rectangle(0, 0, 0, 0); }
			Rectangle(int left, int top, int width, int height) { this->left = left; this->top = top; SetWidth(width); SetHeight(height); }
			Rectangle(const Rectangle &rect) { left = rect.left; top = rect.top; SetWidth(rect.width); SetHeight(rect.height); }
			
			void Offset(int left, int top)
			{
				SetLeft(this->left + left);
				SetTop(this->top + top);
			}

			Rectangle OffsetEx(int left, int top)
			{
				return Rectangle(this->left + left, this->top + top, width, height);
			}
			
			void Inflate(int width, int height)
			{
				SetWidth(this->width + width);
				SetHeight(this->height + height);
			}

			Rectangle InflateEx(int width, int height)
			{
				return Rectangle(left, top, this->width + width, this->height + height);
			}
			
			bool Contains(const Point &point)
			{
				return (left <= point.Left && point.Left <= right) && (top <= point.Top && point.Top <= bottom);
			}
			
			Point GetPosition()
			{
				return Point(left, top);
			}
			
			Size GetSize()
			{
				return Size(width, height);
			}
		};
	}
}

#endif