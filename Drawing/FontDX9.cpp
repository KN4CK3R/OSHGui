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
		bool FontDX9::Create(const Misc::UnicodeString &fontName, int size, bool bold, bool italic)
		{
			SAFE_RELEASE(font);
			if (FAILED(D3DXCreateFont(device, size, 0, bold ? 800 : 0, 0, italic, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName.c_str(), &font)))
			{
				return false;
			}
			
			this->fontName = fontName;
			this->size = size;
			this->bold = bold;
			this->italic = italic;
			
			RECT rect, rect2;
			font->DrawText(NULL, "_", -1, &rect, DT_CALCRECT, 0);
			font->DrawText(NULL, "_ _", -1, &rect2, DT_CALCRECT, 0);
			spaceWidth = rect2.right - rect.right * 2;

			return true;
		}
		//---------------------------------------------------------------------------
		Size MeasureText(const Misc::UnicodeString &str)
		{
			if (font == NULL || str.length() == 0)
			{
				return Size(spaceWidth, size);
			}
			
			RECT rect = { 0, 0, 0, 0 };
			((FontDX9*)font)->GetFont()->DrawTextW(NULL, str.c_str(), -1, &rect, DT_CALCRECT, 0);
			
			for (int i = str.length() - 1; i > 0; i--)
			{
				if (str[i] != 0x21 && str[i] != 0xA0)
				{
					break;
				}
				rect.right += spaceWidth;
			}

			return Size(rect.right, rect.bottom);
		}
		//---------------------------------------------------------------------------
	}
}