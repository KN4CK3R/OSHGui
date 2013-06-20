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
			: fontID(-1)
		{
			Create(name, size, bold, italic);
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
		void FontOpenGL::Create(const Misc::AnsiString &name, int size, bool bold, bool italic)
		{
			this->name = name;
			this->size = size;
			this->bold = bold;
			this->italic = italic;

			fontID = glGenLists(256);

			HDC dc = wglGetCurrentDC();
			font = CreateFontA(size, 0, 0, 0, bold ? FW_HEAVY : 0, italic, 0, 0, ANSI_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FW_DONTCARE, name.c_str());
			HFONT oldFont = static_cast<HFONT>(SelectObject(dc, font));
			wglUseFontBitmapsA(dc, 0, 255, fontID);
			SelectObject(dc, oldFont);
		}
		//---------------------------------------------------------------------------
 		const Size FontOpenGL::MeasureText(const Misc::AnsiString &str)
 		{
			HDC dc = wglGetCurrentDC();
			HFONT oldFont = static_cast<HFONT>(SelectObject(dc, font));
			SIZE size;
			GetTextExtentPoint32(dc, str.c_str(), str.length(), &size);
			SelectObject(dc, oldFont);

 			return Size(size.cx, size.cy);
 		}
		//---------------------------------------------------------------------------
	}
}