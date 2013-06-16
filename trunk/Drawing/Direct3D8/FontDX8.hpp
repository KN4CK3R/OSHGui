/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_FONTDX8_HPP
#define OSHGUI_DRAWING_FONTDX8_HPP

#include "../IFont.hpp"

struct ID3DXFont;
struct IDirect3DDevice8;

namespace OSHGui
{
	namespace Drawing
	{
		class OSHGUI_EXPORT FontDX8 : public IFont
		{
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 */
			FontDX8(IDirect3DDevice8 *device, const Misc::AnsiString &fontName, int size, bool bold, bool italic);
			virtual ~FontDX8();
			
			/**
			 * Ruft das zugrundeliegende IDirect3DFont-Objekt ab.
			 */
			ID3DXFont* GetFont();
			
			/**
			 * Ruft die Ma�e des Texts mit dieser Schriftart ab.
			 *
			 * @param str der Text
			 * @return die Ma�e
			 */
			virtual const Size MeasureText(const Misc::AnsiString &str);
			
			void PreReset();
			
			void PostReset();

		protected:
			/**
			 * Erzeugt eine neue Schriftart.
			 *
			 * @param fontName der Name der Schriftart
			 * @param size die Schriftgr��e
			 * @param bold fettgedruckt
			 * @param italic kursiv
			 */
			virtual void Create(const Misc::AnsiString &fontName, int size, bool bold, bool italic);
		
			IDirect3DDevice8 *device;
			ID3DXFont *font;
			
			int spaceWidth;
		};
	}
}

#endif