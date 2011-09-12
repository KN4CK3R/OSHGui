#include "FontDX10.h"

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
		bool FontDX10::Create(const Misc::UnicodeString &fontName, int size, bool bold, bool italic)
		{
			SAFE_RELEASE(font);
			if (FAILED(D3DX10CreateFont(device, size, 0, bold ? 800 : 0, 0, italic, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName.c_str(), &font)))
			{
				return false;
			}
			
			this->fontName = fontName;
			this->size = size;
			this->bold = bold;
			this->italic = italic;

			return true;
		}
		//---------------------------------------------------------------------------
	}
}