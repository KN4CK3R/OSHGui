#include "FontOpenGL.h"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		FontOpenGL::FontOpenGL()
		{
			fontID = -1;
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
		GLuint FontOpenGL::GetFont()
		{
			return fontID;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		bool FontOpenGL::Create(const String &fontName, int size, bool bold, bool italic)
		{
			if (fontName == NULL)
			{
				return false;
			}

			HDC hDC = wglGetCurrentDC();
			fontID = glGenLists(256);
			HFONT hFont = CreateFont(size, 0, 0, 0, bold ? FW_HEAVY : 0, italic, 0, 0, ANSI_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FW_DONTCARE, fontName.c_str());
			HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
			wglUseFontBitmaps(hDC, 0, 255, fontID);
			SelectObject(hDC, hOldFont);
			DeleteObject(hFont);

			return true;
		}
		//---------------------------------------------------------------------------
		int FontOpenGL::MeasureCharacter(Char c)
		{
			//todo
			return 0;
		}
		//---------------------------------------------------------------------------
	}
}