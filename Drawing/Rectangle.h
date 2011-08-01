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
		public:
			Rectangle();
			Rectangle(int left, int top, int width, int height);
			Rectangle(const Rectangle &rect);
			
			void SetLeft(int left);
			const int GetLeft();
			void SetTop(int top);
			int GetTop();
			void SetWidth(int width);
			int GetWidth();
			void SetHeight(int height);
			int GetHeight();
			int GetRight();
			int GetBottom();
			Point GetPosition();
			Size GetSize();
			
			bool operator == (const Rectangle &equal);
			bool operator != (const Rectangle &equal);
			
			void Offset(int left, int top);
			Rectangle OffsetEx(int left, int top);
			void Inflate(int width, int height);
			Rectangle InflateEx(int width, int height);
			bool Contains(const Point &point);

		private:
			int left,
				top,
				width,
				height;
			int bottom,
				right;
		};
	}
}

#endif