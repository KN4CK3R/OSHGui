/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
		SizeF::SizeF()
			: Width(0), Height(0)
		{
			
		}
		//---------------------------------------------------------------------------
		SizeF::SizeF(int width, int height)
			: Width(width), Height(height)
		{

		}
		//---------------------------------------------------------------------------
		//Operator
		//---------------------------------------------------------------------------
		const SizeF SizeF::operator - (const SizeF &s) const
		{
			return InflateEx(-s.Width, -s.Height);
		}
		//---------------------------------------------------------------------------
		SizeF& SizeF::operator -= (const SizeF &s)
		{
			Inflate(-s.Width, -s.Height);
			return *this;
		}
		//---------------------------------------------------------------------------
		const SizeF SizeF::operator + (const SizeF &s) const
		{
			return InflateEx(s.Width, s.Height);
		}
		//---------------------------------------------------------------------------
		SizeF& SizeF::operator += (const SizeF &s)
		{
			Inflate(s.Width, s.Height);
			return *this;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void SizeF::Inflate(int width, int height)
		{
			this->Width += width;
			this->Height += height;
		}
		//---------------------------------------------------------------------------
		SizeF SizeF::InflateEx(int width, int height) const
		{
			return SizeF(this->Width + width, this->Height + height);
		}
		//---------------------------------------------------------------------------
		bool operator==(const SizeF &lhs, const SizeF &rhs)
		{
			return lhs.Width == rhs.Width && lhs.Height == rhs.Height;
		}
		//---------------------------------------------------------------------------
		bool operator<(const SizeF &lhs, const SizeF &rhs)
		{
			return lhs.Width < rhs.Width && lhs.Height < rhs.Height;
		}
		//---------------------------------------------------------------------------
	}
}