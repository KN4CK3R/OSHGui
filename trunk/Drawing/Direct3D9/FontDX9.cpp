/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#include "FontDX9.hpp"
#include "../../Misc/Exceptions.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		FontDX9::FontDX9(IDirect3DDevice9 *device, const Misc::AnsiString &name, int size, bool bold, bool italic)
		{
			this->device = device;
			
			if (FAILED(D3DXCreateFontA(device, size, 0, bold ? 800 : 0, 0, italic, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &font)))
			{
				font = nullptr;
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw Misc::Exception("Cannot create Font.", __FILE__, __LINE__);
				#else
				throw 1;
				#endif
			}

			this->name = name;
			this->size = size;
			this->bold = bold;
			this->italic = italic;

			RECT rect = { 0, 0, 0, 0},
				 rect2 = { 0, 0, 0, 0 };
			font->DrawTextA(0, "_", -1, &rect, DT_CALCRECT, 0);
			font->DrawTextA(0, "_ _", -1, &rect2, DT_CALCRECT, 0);
			spaceWidth = rect2.right - rect.right * 2;
		}
		//---------------------------------------------------------------------------
		FontDX9::~FontDX9()
		{
			if (font != nullptr)
			{
				font->Release();
			}
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		ID3DXFont* FontDX9::GetFont()
		{
			return font;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void FontDX9::Create(const Misc::AnsiString &fontName, int size, bool bold, bool italic)
		{
			
		}
		//---------------------------------------------------------------------------
		const Size FontDX9::MeasureText(const Misc::AnsiString &str)
		{
			if (str.length() == 0)
			{
				return Size(0, size);
			}

			RECT rect = { 0, 0, 0, 0 };
			font->DrawTextA(0, str.c_str(), str.length(), &rect, DT_CALCRECT, 0xFF000000);
			
			for (int i = str.length() - 1; i > 0; --i)
			{
				if (str[i] != ' ')
				{
					break;
				}
				rect.right += spaceWidth;
			}

			return Size(rect.right, rect.bottom);
		}
		//---------------------------------------------------------------------------
		void FontDX9::PreReset()
		{
			font->OnLostDevice();
		}
		//---------------------------------------------------------------------------
		void FontDX9::PostReset()
		{
			font->OnResetDevice();
		}
		//---------------------------------------------------------------------------
	}
}