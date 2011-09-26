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
		bool Size::operator == (const Size &equal) const
		{
			return Width == equal.Width && Height == equal.Height;
		}
		//---------------------------------------------------------------------------
		bool Size::operator != (const Size &equal) const
		{
			return Width != equal.Width || Height != equal.Height;
		}
		//---------------------------------------------------------------------------
		const Size Size::operator - (const Size &s) const
		{
			return InflateEx(-s.Width, -s.Height);
		}
		//---------------------------------------------------------------------------
		Size& Size::operator -= (const Size &s)
		{
			Inflate(-s.Width, -s.Height);
			return *this;
		}
		//---------------------------------------------------------------------------
		const Size Size::operator + (const Size &s) const
		{
			return InflateEx(s.Width, s.Height);
		}
		//---------------------------------------------------------------------------
		Size& Size::operator += (const Size &s)
		{
			Inflate(s.Width, s.Height);
			return *this;
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
		Size Size::InflateEx(int width, int height) const
		{
			return Size(this->Width + width, this->Height + height);
		}
		//---------------------------------------------------------------------------
	}
}