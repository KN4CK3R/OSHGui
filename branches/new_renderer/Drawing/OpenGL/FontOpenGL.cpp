/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "FontOpenGL.hpp"
#include <gl/glut.h>

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		FontOpenGL::FontOpenGL(const Misc::AnsiString &name, int size, bool bold, bool italic)
			: IFont(name, size, bold, italic),
			  fontID(-1)
		{
			fontID = glGenLists(256);

			auto hdc = wglGetCurrentDC();
			font = CreateFontA(GetSize(), 0, 0, 0, IsBold() ? FW_HEAVY : 0, IsItalic(), 0, 0, ANSI_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FW_DONTCARE, GetName().c_str());
			auto oldFont = static_cast<HFONT>(SelectObject(hdc, font));
			wglUseFontBitmapsA(hdc, 0, 255, fontID);
			SelectObject(hdc, oldFont);
		}
		//---------------------------------------------------------------------------
		FontOpenGL::~FontOpenGL()
		{
			if (fontID != -1)
			{
				glDeleteLists(fontID, 256);
				DeleteObject(font);
			}
		}
		//Getter/Setter
		//---------------------------------------------------------------------------
		unsigned int FontOpenGL::GetFont()
		{
			return fontID;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
 		const SizeF FontOpenGL::MeasureText(const Misc::AnsiString &str)
 		{
			auto hdc = wglGetCurrentDC();
			HFONT oldFont = static_cast<HFONT>(SelectObject(hdc, font));
			SIZE size;
			GetTextExtentPoint32(hdc, str.c_str(), str.length(), &size);
			SelectObject(hdc, oldFont);

 			return SizeF(size.cx, size.cy);
 		}
		//---------------------------------------------------------------------------
	}
}