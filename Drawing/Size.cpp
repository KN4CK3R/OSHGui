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
			Size(0, 0);
		}
		//---------------------------------------------------------------------------
		Size::Size(int Width, int Height)
		{
			this->Width = Width;
			this->Height = Height;
		}
		//---------------------------------------------------------------------------
		Size::Size(const Size &s)
		{
			Width = s.Width;
			Height = s.Height;
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
		void Size::Inflate(int Width, int Height)
		{
			this->Width += Width;
			this->Height += Height;
		}
		//---------------------------------------------------------------------------
		Size Size::InflateEx(int Width, int Height)
		{
			return Size(this->Width + Width, this->Height + Height);
		}
		//---------------------------------------------------------------------------
	}
}