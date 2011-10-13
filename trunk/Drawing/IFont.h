#ifndef OSHGUI_DRAWING_IFONT_H_
#define OSHGUI_DRAWING_IFONT_H_

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
			virtual const Size MeasureText(const Misc::UnicodeString &text) = 0;
			
		protected:
			/**
			 * Erzeugt eine neue Schriftart.
			 *
			 * @param fontName der Name der Schriftart
			 * @param size die Schriftgr��e
			 * @param bold fettgedruckt
			 * @param italic kursiv
			 */
			virtual void Create(const Misc::UnicodeString &fontName, int size, bool bold, bool italic) = 0;
		
			Misc::UnicodeString fontName;
			int size;
			bool bold;
			bool italic;
		};
	}
}

#endif