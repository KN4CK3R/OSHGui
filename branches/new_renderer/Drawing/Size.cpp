/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Size.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Size::Size()
			: Width(0), Height(0)
		{
			
		}
		//---------------------------------------------------------------------------
		Size::Size(int width, int height)
			: Width(width), Height(height)
		{

		}
		//---------------------------------------------------------------------------
		//Operator
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
		bool operator==(const Size &lhs, const Size &rhs)
		{
			return lhs.Width == rhs.Width && lhs.Height == rhs.Height;
		}
		//---------------------------------------------------------------------------
		bool operator<(const Size &lhs, const Size &rhs)
		{
			return lhs.Width < rhs.Width && lhs.Height < rhs.Height;
		}
		//---------------------------------------------------------------------------
	}
}