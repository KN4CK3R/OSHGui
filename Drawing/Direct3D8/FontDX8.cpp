/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include <d3dx8.h>
#include "FontDX8.hpp"
#include "../../Misc/Exceptions.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		FontDX8::FontDX8(IDirect3DDevice8 *device, const Misc::AnsiString &name, int size, bool bold, bool italic)
		{
			this->device = device;
			
			HFONT hfont = CreateFontA(size, 0, 0, 0, bold ? 800 : 0, italic, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str());

			if (FAILED(D3DXCreateFont(device, hfont, &font)))
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
			font->DrawTextA("_", -1, &rect, DT_CALCRECT, 0);
			font->DrawTextA("_ _", -1, &rect2, DT_CALCRECT, 0);
			spaceWidth = rect2.right - rect.right * 2;
		}
		//---------------------------------------------------------------------------
		FontDX8::~FontDX8()
		{
			if (font != nullptr)
			{
				font->Release();
			}
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		ID3DXFont* FontDX8::GetFont()
		{
			return font;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void FontDX8::Create(const Misc::AnsiString &fontName, int size, bool bold, bool italic)
		{
			
		}
		//---------------------------------------------------------------------------
		const Size FontDX8::MeasureText(const Misc::AnsiString &str)
		{
			if (str.length() == 0)
			{
				return Size(0, size);
			}

			RECT rect = { 0, 0, 0, 0 };
			font->DrawTextA(str.c_str(), str.length(), &rect, DT_CALCRECT, 0xFF000000);
			
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
		void FontDX8::PreReset()
		{
			font->OnLostDevice();
		}
		//---------------------------------------------------------------------------
		void FontDX8::PostReset()
		{
			font->OnResetDevice();
		}
		//---------------------------------------------------------------------------
	}
}