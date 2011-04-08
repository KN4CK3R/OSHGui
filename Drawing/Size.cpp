#include "Size.h"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Size()
		{
			Size(0, 0);
		}
		//---------------------------------------------------------------------------
		Size(int Width, int Height)
		{
			this->Width = Width;
			this->Height = Height;
		}
		//---------------------------------------------------------------------------
		Size(const Size &s)
		{
			Width = s.Width;
			Height = s.Height;
		}
		//---------------------------------------------------------------------------
		//Operator
		//---------------------------------------------------------------------------
		bool operator == (const Size &equal)
		{
			return Width == equal.Width && Height == equal.Height;
		}
		//---------------------------------------------------------------------------
		bool operator != (const Size &equal)
		{
			return Width != equal.Width || Height != equal.Height;
		}
		//---------------------------------------------------------------------------
		Size operator - (const Size &s)
		{
			return InflateEx(-s.Width, -s.Height);
		}
		//---------------------------------------------------------------------------
		void operator -= (const Size &s)
		{
			Inflate(-s.Width, -s.Height);
		}
		//---------------------------------------------------------------------------
		Size operator + (const Size &s)
		{
			return InflateEx(s.Width, s.Height);
		}
		//---------------------------------------------------------------------------
		void operator += (const Size &s)
		{
			Inflate(s.Width, s.Height);
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void Inflate(int Width, int Height)
		{
			this->Width += Width;
			this->Height += Height;
		}
		//---------------------------------------------------------------------------
		Size InflateEx(int Width, int Height)
		{
			return Size(this->Width + Width, this->Height + Height);
		}
		//---------------------------------------------------------------------------
	}
}

#endif