/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "RendererDX10.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		RendererDX10::RendererDX10(ID3D10Device *device)
		{
			this->device = device;
			
			color = 0xFFFFFFFF;
		}
		//---------------------------------------------------------------------------
		RendererDX10::~RendererDX10()
		{
		
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void RendererDX10::DrawTexture(const Drawing::PointF &point, Drawing::ITexture *texture)
		{
			DrawTexture(point.X, point.Y, texture);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::DrawTexture(int x, int y, Drawing::ITexture *texture)
		{
			if (texture == NULL)
			{
				return;
			}
			
			//not implemented
		}
		//---------------------------------------------------------------------------
		Drawing::SizeF RendererDX10::MeasureText(Drawing::IFont *font, const Misc::UnicodeString &text)
		{
			if (font == NULL)
			{
				return SizeF();
			}
			return font->MeasureText(text);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::RenderText(Drawing::IFont *font, const Drawing::PointF &point, const Misc::UnicodeString &text)
		{
			RenderText(point.X, point.Y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::RenderText(Drawing::IFont *font, int x, int y, const Misc::UnicodeString &text)
		{
			RenderText(x, y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::RenderText(Drawing::IFont *font, const Drawing::RectangleF &rectangle, const Misc::UnicodeString &text)
		{
			RenderText(rectangle.GetLeft(), rectangle.GetTop(), rectangle.GetWidth(), rectangle.GetHeight(), text);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::RenderText(Drawing::IFont *font, int x, int y, int w, int h, const Misc::UnicodeString &text)
		{
			if (font == NULL)
			{
				return;
			}
			
			//not implemented
		}
		//---------------------------------------------------------------------------
		void RendererDX10::Fill(const Drawing::PointF &point, D3DCOLOR color)
		{
			Fill(point.X, point.Y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::Fill(int x, int y, D3DCOLOR color)
		{
			Fill(x, y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::Fill(Drawing::RectangleF &rect, D3DCOLOR color)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), color);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::Fill(int x, int y, int w, int h, D3DCOLOR color)
		{
			//not implemented
		}
		//---------------------------------------------------------------------------
	}
}