#include "FontDX9.h"

#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = NULL; } }
#endif

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		FontDX9::FontDX9(IDirect3DDevice9 *device)
		{
			this->device = device;
			
			initWidthArray = false;
			
			font = NULL;
		}
		//---------------------------------------------------------------------------
		FontDX9::~FontDX9()
		{
			SAFE_RELEASE(font);
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		LPD3DXFONT FontDX9::GetFont()
		{
			return font;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		bool FontDX9::Create(LPCSTR fontName, int size, bool bold, bool italic)
		{
			if (fontName == NULL)
			{
				return false;
			}

			SAFE_RELEASE(font);
			if (FAILED(D3DXCreateFont(device, size, 0, bold ? 800 : 0, 0, italic, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName, &font)))
			{
				return false;
			}
			
			initWidthArray = true;

			return true;
		}
		//---------------------------------------------------------------------------
		int FontDX9::MeasureCharacter(char c)
		{
			if (font == NULL)
			{
				return 0;
			}
			
			if (initWidthArray)
			{
				//special chars
				for (int i = 0; i < 0x20; i++)
				{
					charWidth[i] = 0;
				}
				
				{ //space needs extra calculation
					RECT rect, rect2;
					font->DrawText(NULL, "_", -1, &rect, DT_CALCRECT, 0);
					font->DrawText(NULL, "_ _", -1, &rect2, DT_CALCRECT, 0);
					charWidth[0x20] = rect2.right - rect.right * 2;
					charWidth[0xA0] = rect2.right - rect.right * 2;
				}
				
				RECT rect;
				char str[2] = { 0, 0 };
				
				for (int i = 0x21; i < 0x7E; i++)
				{
					str[0] = (char)i;
					font->DrawText(NULL, str, -1, &rect, DT_CALCRECT, 0);
					charWidth[i] = rect.right;
				}
				
				//special chars
				for (int i = 0x7F; i < 0xA0; i++)
				{
					charWidth[i] = 0;
				}
				
				for (int i = 0xA1; i < 0xFF; i++)
				{
					str[0] = (char)i;
					font->DrawText(NULL, str, -1, &rect, DT_CALCRECT, 0);
					charWidth[i] = rect.right;
				}
			}
			
			return charWidth[(int)c];
		}
		//---------------------------------------------------------------------------
	}
}