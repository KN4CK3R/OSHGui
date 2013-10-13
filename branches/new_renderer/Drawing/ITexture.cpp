/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "ITexture.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		ITexture::~ITexture()
		{
			
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		const SizeF& ITexture::GetSize() const
		{
			return size;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void ITexture::Clear()
		{
			Fill(0, 0, size.Width, size.Height, Color::Empty());
		}
		//---------------------------------------------------------------------------
		void ITexture::Clear(const PointF &point)
		{
			Fill(point.X, point.Y, 1, 1, Color::Empty());
		}
		//---------------------------------------------------------------------------
		void ITexture::Clear(int x, int y)
		{
			Fill(x, y, 1, 1, Color::Empty());
		}
		//---------------------------------------------------------------------------
		void ITexture::Clear(const RectangleF &rect)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), Color::Empty());
		}
		//---------------------------------------------------------------------------
		void ITexture::Clear(int x, int y, int w, int h)
		{
			Fill(x, y, w, h, Color::Empty());
		}
		//---------------------------------------------------------------------------
		void ITexture::Fill(Color color)
		{
			Fill(0, 0, size.Width, size.Height, color);
		}
		//---------------------------------------------------------------------------
		void ITexture::Fill(const PointF &point, Color color)
		{
			Fill(point.X, point.Y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void ITexture::Fill(int x, int y, Color color)
		{
			Fill(x, y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void ITexture::Fill(const RectangleF &rect, Color color)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), color);
		}
		//---------------------------------------------------------------------------
		void ITexture::FillGradient(Color from, Color to, bool updown)
		{
			FillGradient(0, 0, size.Width, size.Height, from, to, updown);
		}
		//---------------------------------------------------------------------------
		void ITexture::FillGradient(const RectangleF &rect, Color from, Color to, bool updown)
		{
			FillGradient(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), from, to, updown);
		}
		//---------------------------------------------------------------------------
		const Misc::TimeSpan& ITexture::GetFrameChangeInterval() const
		{
			return frameChangeInterval;
		}
		//---------------------------------------------------------------------------
	}
}