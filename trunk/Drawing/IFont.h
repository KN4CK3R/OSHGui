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
		 * Definiert ein bestimmtes abstraktes Format für Text, einschließlich der Attribute
		 * für Schriftart, Schriftgrad und Schriftschnitt.
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
			 * Ruft die Schriftgröße ab.
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
			 * Ruft die Maße des Texts mit dieser Schriftart ab.
			 *
			 * @param text der Text
			 * @return die Maße
			 */
			virtual const Size MeasureText(const Misc::UnicodeString &text) = 0;
			
		protected:
			/**
			 * Erzeugt eine neue Schriftart.
			 *
			 * @param fontName der Name der Schriftart
			 * @param size die Schriftgröße
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