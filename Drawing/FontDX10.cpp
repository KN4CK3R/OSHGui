#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\FontDX10.h"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		FontDX10::FontDX10(ID3D10Device *device)
		{
			this->device = device;
			
			font = NULL;
		}
		//---------------------------------------------------------------------------
		FontDX10::~FontDX10()
		{
			SAFE_RELEASE(font);
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		LPD3DX10FONT FontDX10::GetFont()
		{
			return font;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		bool FontDX10::Create(LPCSTR fontName, int size, bool bold, bool italic)
		{
			if (fontName == NULL)
			{
				return false;
			}

			SAFE_RELEASE(font);
			if (FAILED(D3DX10CreateFont(device, size, 0, bold ? 800 : 0, 0, italic, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName, &font)))
			{
				return false;
			}

			return true;
		}
		//---------------------------------------------------------------------------
	}
}