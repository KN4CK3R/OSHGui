#include "Size.h"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Size::Size()
		{
			Width = 0;
			Height = 0;
		}
		//---------------------------------------------------------------------------
		Size::Size(int width, int height)
		{
			this->Width = width;
			this->Height = height;
		}
		//---------------------------------------------------------------------------
		//Operator
		//---------------------------------------------------------------------------
		bool Size::operator == (const Size &equal)
		{
			return Width == equal.Width && Height == equal.Height;
		}
		//---------------------------------------------------------------------------
		bool Size::operator != (const Size &equal)
		{
			return Width != equal.Width || Height != equal.Height;
		}
		//---------------------------------------------------------------------------
		Size Size::operator - (const Size &s)
		{
			return InflateEx(-s.Width, -s.Height);
		}
		//---------------------------------------------------------------------------
		void Size::operator -= (const Size &s)
		{
			Inflate(-s.Width, -s.Height);
		}
		//---------------------------------------------------------------------------
		Size Size::operator + (const Size &s)
		{
			return InflateEx(s.Width, s.Height);
		}
		//---------------------------------------------------------------------------
		void Size::operator += (const Size &s)
		{
			Inflate(s.Width, s.Height);
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void Size::Inflate(int width, int height)
		{
			this->Width += width;
			this->Height += height;
		}
		//---------------------------------------------------------------------------
		Size Size::InflateEx(int width, int height)
		{
			return Size(this->Width + width, this->Height + height);
		}
		//---------------------------------------------------------------------------
		bool Size::Contains(const Point &point)
		{
			return (0 <= point.Left && point.Left <= Width) && (0 <= point.Top && point.Top <= Height);
		}
		//---------------------------------------------------------------------------
	}
}