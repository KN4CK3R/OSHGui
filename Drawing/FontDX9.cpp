#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\FontDX9.h"

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
		int FontDX9::MeasureChar(char c)
		{
			if (font == NULL)
			{
				return 0;
			}
			
			if (initWidthArray)
			{
				// \0 char is 0
				charWidth[0] = 0;
			
				RECT rct, rct2;
				char str[2] = { 0, 0 };
				
				for (int i = 1; i < 0xFF; i++)
				{
					if (i == 0x20) // space needs extra calculation
					{
						font->DrawText(NULL, "_", -1, &rct, DT_CALCRECT, 0);
						font->DrawText(NULL, "_ _", -1, &rct2, DT_CALCRECT, 0);
						charWidth[i] = rct2.right - rct.right * 2;
					}
					else
					{
						str[0] = (char)i;
						font->DrawText(NULL, str, -1, &rct, DT_CALCRECT, 0);
						charWidth[i] = rct.right;
					}
				}
			}
			
			return charWidth[(int)c];
		}
		//---------------------------------------------------------------------------
	}
}