#include "RendererDX11.h"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		RendererDX11::RendererDX11(ID3D11Device *device)
		{
			this->device = device;
			
			color = 0xFFFFFFFF;
		}
		//---------------------------------------------------------------------------
		RendererDX11::~RendererDX11()
		{
		
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void RenderDX11::SetRenderColor(D3DCOLOR color)
		{
			this->color = color;
		}
		//---------------------------------------------------------------------------
		void RendererDX11::DrawTexture(const Drawing::Point &point, Drawing::ITexture *texture)
		{
			DrawTexture(point.X, point.Y, texture);
		}
		//---------------------------------------------------------------------------
		void RendererDX11::DrawTexture(int x, int y, Drawing::ITexture *texture)
		{
			if (texture == NULL)
			{
				return;
			}
		}
		//---------------------------------------------------------------------------
		Drawing::Size RendererDX11::MeasureText(Drawing::IFont *font, const Misc::UnicodeString &text)
		{
			if (font == NULL)
			{
				return Size();
			}
			return font->MeasureText(text);
		}
		//---------------------------------------------------------------------------
		void RendererDX11::RenderText(Drawing::IFont *font, const Drawing::Point &point, const Misc::UnicodeString &text)
		{
			RenderText(point.X, point.Y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX11::RenderText(Drawing::IFont *font, int x, int y, const Misc::UnicodeString &text)
		{
			RenderText(x, y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX11::RenderText(Drawing::IFont *font, const Drawing::Rectangle &rectangle, const Misc::UnicodeString &text)
		{
			RenderText(rectangle.GetLeft(), rectangle.GetTop(), rectangle.GetWidth(), rectangle.GetHeight(), text);
		}
		//---------------------------------------------------------------------------
		void RendererDX11::RenderText(Drawing::IFont *font, int x, int y, int w, int h, const Misc::UnicodeString &text)
		{
			if (font == NULL)
			{
				return;
			}
		}
		//---------------------------------------------------------------------------		
		void RendererDX11::Fill(const Drawing::Point &point, D3DCOLOR color)
		{
			Fill(point.X, point.Y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void RendererDX11::Fill(int x, int y, D3DCOLOR color)
		{
			Fill(x, y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void RendererDX11::Fill(Drawing::Rectangle &rect, D3DCOLOR color)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), color);
		}
		//---------------------------------------------------------------------------
		void RendererDX11::Fill(int x, int y, int w, int h, D3DCOLOR color)
		{
		
		}
		//---------------------------------------------------------------------------
	}
}