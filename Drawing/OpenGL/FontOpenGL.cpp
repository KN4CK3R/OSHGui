/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "FontOpenGL.hpp"
#include <windows.h>
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

			HDC hDC = wglGetCurrentDC();
			fontID = glGenLists(256);
			HFONT hFont = CreateFontA(size, 0, 0, 0, bold ? FW_HEAVY : 0, italic, 0, 0, ANSI_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FW_DONTCARE, name.c_str());
			/*HFONT hOldFont = (HFONT)*/SelectObject(hDC, hFont);
			wglUseFontBitmaps(hDC, 0, 255, fontID);
			//SelectObject(hDC, hOldFont);
			//DeleteObject(hFont);
		}
		//---------------------------------------------------------------------------
 		const Size FontOpenGL::MeasureText(const Misc::AnsiString &str)
 		{
 			return Size();
 		}
		//---------------------------------------------------------------------------
	}
}