#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\RendererDX10.h"

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
		void RendererDX10::DrawTexture(const Drawing::Point &point, Drawing::ITexture *texture)
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
		}
		//---------------------------------------------------------------------------
		Drawing::Size RendererDX10::MeasureText(Drawing::IFont *font, LPCSTR text)
		{
			if (font == NULL || text == NULL)
			{
				return Size();
			}
			return font->MeasureText(text);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::DrawText(Drawing::IFont *font, const Drawing::Point &point, LPCSTR text)
		{
			DrawText(point.X, point.Y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::DrawText(Drawing::IFont *font, int x, int y, LPCSTR text)
		{
			DrawText(x, y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::DrawText(Drawing::IFont *font, const Drawing::Rectangle &rectangle, LPCSTR text)
		{
			DrawText(rectangle.GetLeft(), rectangle.GetTop(), rectangle.GetWidth(), rectangle.GetHeight(), text);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::DrawText(Drawing::IFont *font, int x, int y, int w, int h, LPCSTR text)
		{
			if (font == NULL || text == NULL)
			{
				return;
			}
		}
		//---------------------------------------------------------------------------		
		void RendererDX10::Fill(const Drawing::Point &point, D3DCOLOR color)
		{
			Fill(point.X, point.Y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::Fill(int x, int y, D3DCOLOR color)
		{
			Fill(x, y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::Fill(Drawing::Rectangle &rect, D3DCOLOR color)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), color);
		}
		//---------------------------------------------------------------------------
		void RendererDX10::Fill(int x, int y, int w, int h, D3DCOLOR color)
		{
		
		}
		//---------------------------------------------------------------------------
	}
}