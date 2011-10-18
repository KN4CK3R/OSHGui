#include "FontDX9.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		FontDX9::FontDX9(IDirect3DDevice9 *device, const Misc::UnicodeString &fontName, int size, bool bold, bool italic)
		{
			this->device = device;
						
			Create(fontName, size, bold, italic);
		}
		//---------------------------------------------------------------------------
		FontDX9::~FontDX9()
		{
			if (font != 0)
			{
				font->Release();
			}
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
		void FontDX9::Create(const Misc::UnicodeString &fontName, int size, bool bold, bool italic)
		{			
			if (FAILED(D3DXCreateFontW(device, size, 0, bold ? 800 : 0, 0, italic, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName.c_str(), &font)))
			{
				font = 0;
			
				throw Misc::Exception(L"Cannot create Font.", __WFILE__, __LINE__);
			}
			
			this->fontName = fontName;
			this->size = size;
			this->bold = bold;
			this->italic = italic;
			
			RECT rect = { 0, 0, 0, 0},
				 rect2 = { 0, 0, 0, 0 };
			font->DrawTextW(0, L"_", -1, &rect, DT_CALCRECT, 0);
			font->DrawTextW(0, L"_ _", -1, &rect2, DT_CALCRECT, 0);
			spaceWidth = rect2.right - rect.right * 2;
		}
		//---------------------------------------------------------------------------
		const Size FontDX9::MeasureText(const Misc::UnicodeString &str)
		{
			if (str.length() == 0)
			{
				return Size(0, size);
			}
			
			RECT rect = { 0, 0, 0, 0 };
			font->DrawTextW(0, str.c_str(), -1, &rect, DT_CALCRECT, 0);
			
			for (int i = str.length() - 1; i > 0; --i)
			{
				if (str[i] != L' ')
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