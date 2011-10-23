#ifndef OSHGUI_DRAWING_FONTDX9_HPP_
#define OSHGUI_DRAWING_FONTDX9_HPP_

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "../IFont.hpp"

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
			FontDX9(IDirect3DDevice9 *device, const Misc::UnicodeString &fontName, int size, bool bold, bool italic);
			~FontDX9();
			
			/**
			 * Ruft das zugrundeliegende IDirect3DFont-Objekt ab.
			 */
			LPD3DXFONT GetFont();
			
			/**
			 * Ruft die Ma�e des Texts mit dieser Schriftart ab.
			 *
			 * @param str der Text
			 * @return die Ma�e
			 */
			virtual const Size MeasureText(const Misc::UnicodeString &str);

		protected:
			/**
			 * Erzeugt eine neue Schriftart.
			 *
			 * @param fontName der Name der Schriftart
			 * @param size die Schriftgr��e
			 * @param bold fettgedruckt
			 * @param italic kursiv
			 */
			virtual void Create(const Misc::UnicodeString &fontName, int size, bool bold, bool italic);
		
			IDirect3DDevice9 *device;
			LPD3DXFONT font;
			
			int spaceWidth;
		};
	}
}

#endif