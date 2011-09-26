#ifndef __OSHGUI_DRAWING_IFONT_H__
#define __OSHGUI_DRAWING_IFONT_H__

#include "Point.h"
#include "Size.h"
#include "Rectangle.h"
#include "..\Misc\Strings.h"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Definiert ein bestimmtes abstraktes Format f�r Text, einschlie�lich der Attribute
		 * f�r Schriftart, Schriftgrad und Schriftschnitt.
		 */
		class IFont
		{
		public:
			/**
			 * Erzeugt eine neue Schriftart.
			 *
			 * @param fontName der Name der Schriftart
			 * @param size die Schriftgr��e
			 * @param bold fettgedruckt
			 * @param italic kursiv
			 */
			virtual bool Create(const Misc::UnicodeString &fontName, int size, bool bold, bool italic) = 0;
			
			/**
			 * Ruft den Namen der Schriftart ab.
			 *
			 * @return der Name
			 */
			const Misc::UnicodeString& GetName() const { return fontName; }
			/**
			 * Ruft die Schriftgr��e ab.
			 */
			int GetSize() const { return size; }
			/**
			 * Ruft ab, ob die Schrift fettgedruckt dargestellt wird.
			 */
			bool IsBold() const { return bold; }
			/**
			 * Ruft ab, ob die Schrift kursiv dargestellt wird.
			 */
			bool IsItalic() const { return italic; }
			
			/**
			 * Ruft die Ma�e des Texts mit dieser Schriftart ab.
			 *
			 * @param text der Text
			 * @return die Ma�e
			 */
			virtual Size MeasureText(const Misc::UnicodeString &text) = 0;
			
		protected:
			Misc::UnicodeString fontName;
			int size;
			bool bold;
			bool italic;
		};
	}
}

#endif