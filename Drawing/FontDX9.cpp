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
		bool FontDX9::Create(LPCWSTR fontName, int size, bool bold, bool italic)
		{
			if (fontName == NULL)
			{
				return false;
			}

			SAFE_RELEASE(font);
			if (FAILED(D3DXCreateFontW(device, size, 0, bold ? 800 : 0, 0, italic, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName, &font)))
			{
				return false;
			}

			return true;
		}
		//---------------------------------------------------------------------------
	}
}