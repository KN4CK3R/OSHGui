#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\FontDX11.h"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		FontDX11::FontDX11(ID3D11Device *device)
		{
			this->device = device;
			
			font = NULL;
		}
		//---------------------------------------------------------------------------
		FontDX11::~FontDX11()
		{
			SAFE_RELEASE(font);
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		LPD3DX11FONT FontDX11::GetFont()
		{
			return font;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		bool FontDX11::Create(LPCSTR fontName, int size, bool bold, bool italic)
		{
			if (fontName == NULL)
			{
				return false;
			}

			SAFE_RELEASE(font);
			if (FAILED(D3DX11CreateFont(device, size, 0, bold ? 800 : 0, 0, italic, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName, &font)))
			{
				return false;
			}

			return true;
		}
		//---------------------------------------------------------------------------
	}
}