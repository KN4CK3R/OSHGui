/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_FONTDX9_HPP
#define OSHGUI_DRAWING_FONTDX9_HPP

#include "../IFont.hpp"

struct ID3DXFont;
struct IDirect3DDevice9;

namespace OSHGui
{
	namespace Drawing
	{
		class OSHGUI_EXPORT FontDX9 : public IFont
		{
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 */
			FontDX9(IDirect3DDevice9 *device, const Misc::AnsiString &fontName, int size, bool bold, bool italic);
			virtual ~FontDX9();
			
			/**
			 * Ruft das zugrundeliegende IDirect3DFont-Objekt ab.
			 */
			ID3DXFont* GetFont();
			
			/**
			 * Ruft die Maﬂe des Texts mit dieser Schriftart ab.
			 *
			 * @param str der Text
			 * @return die Maﬂe
			 */
			virtual const Size MeasureText(const Misc::AnsiString &str);
			
			void PreReset();
			
			void PostReset();

		private:
			IDirect3DDevice9 *device;
			ID3DXFont *font;
			
			int spaceWidth;
		};
	}
}

#endif