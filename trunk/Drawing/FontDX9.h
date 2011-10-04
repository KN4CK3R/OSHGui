#ifndef OSHGUI_DRAWING_FONTDX9_H_
#define OSHGUI_DRAWING_FONTDX9_H_

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "IFont.h"

namespace OSHGui
{
	namespace Drawing
	{
		class FontDX9 : public IFont
		{
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * @device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 */
			FontDX9(IDirect3DDevice9 *device);
			~FontDX9();
			
			/**
			 * Ruft das zugrundeliegende IDirect3DFont-Objekt ab.
			 */
			LPD3DXFONT GetFont();

			/**
			 * Erzeugt eine neue Schriftart.
			 *
			 * @param fontName der Name der Schriftart
			 * @param size die Schriftgrˆﬂe
			 * @param bold fettgedruckt
			 * @param italic kursiv
			 */
			virtual bool Create(const Misc::UnicodeString &fontName, int size, bool bold, bool italic);
			
			/**
			 * Ruft die Maﬂe des Texts mit dieser Schriftart ab.
			 *
			 * @param text der Text
			 * @return die Maﬂe
			 */
			virtual Size MeasureText(const Misc::UnicodeString &str);

		protected:
			IDirect3DDevice9 *device;
			LPD3DXFONT font;
			
			int spaceWidth;
		};
	}
}

#endif