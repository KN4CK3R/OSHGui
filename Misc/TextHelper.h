#ifndef __OSHGUI_MISC_TEXTHELPER_H__
#define __OSHGUI_MISC_TEXTHELPER_H__

#include "Misc\Strings.h"
#include "Drawing\IFont.h"
#include "Drawing\Point.h"
#include "Drawing\Size.h"

namespace OSHGui
{
	namespace Misc
	{
		/**
		 * Verwaltet eine Zeichenkette und deren Ausmaße.
		 */
		class TextHelper
		{
		public:
			/**
			 * Erstellt ein TextHelper-Objekt mit der entsprechenden Schriftart.
			 *
			 * @param font darf nicht NULL sein
			 */
			TextHelper(Drawing::IFont *font);
			
			/**
			 * Legt die Schriftart fest.
			 *
			 * @param font darf nicht NULL sein
			 */
			void SetFont(Drawing::IFont *font);
			/**
			 * Legt den Text fest.
			 *
			 * @param text
			 */
			void SetText(const UnicodeString &text);
			/**
			 * Hängt das Zeichen an den Text an.
			 *
			 * @param character
			 */
			void Append(const UnicodeChar character);
			/**
			 * Hängt die Zeichenkette an den Text an.
			 *
			 * @param text
			 */
			void Append(const UnicodeString &text);
			/**
			 * Fügt das Zeichen an der entsprechenden Position im Text ein.
			 *
			 * @param position
			 * @param character
			 */
			void Insert(int position, const UnicodeChar character);
			/**
			 * Fügt die Zeichenkette an der entsprechenden Position im Text ein.
			 *
			 * @param position
			 * @param text
			 */
			void Insert(int position, const UnicodeString &text);
			/**
			 * Löscht den kompletten Text.
			 */
			void Clear();
			/**
			 * Löscht das Zeichen an der entsprechenden Stelle.
			 *
			 * @param length Anzahl der zu löschenden Zeichen (default: 1)
			 */
			void Remove(int index, int length = 1);
			
			/**
			 * Ruft die Länge der Zeichenkette ab.
			 *
			 * @return length
			 */
			int GetLength() const;
			/**
			 * Ruft die Zeichenkette ab.
			 *
			 * @return text
			 */
			const UnicodeString& GetText() const;
			/**
			 * Ruft die Maße der Zeichenkette in Pixel ab.
			 *
			 * @return size
			 */
			const Drawing::Size& GetSize() const;
			
			/**
			 * Berechnet die Maße neu.
			 */
			void RefreshSize();
			
			/**
			 * Gibt die Position des Zeichens an der entsprechenden Stelle zurück.
			 *
			 * @param index
			 * @param trailing linker Rand des Zeichens / rechter Rand des Zeichens
			 * @return position
			 */
			Drawing::Point GetCharacterPosition(int index, bool trailing = false);
			/**
			 * Gibt die Weite eines Teilstrings zurück.
			 * 
			 * @param index der Startindex
			 * @param size die Länge der Zeichenkette. -1 = bis zum Ende (default: -1)
			 * @return die Größe
			 */
			Drawing::Size GetStringWidth(int index, int size = -1);
			/**
			 * Gibt den Index des Zeichens zurück, das der Position am nächsten steht.
			 *
			 * @param position
			 * @return index
			 */
			int GetClosestCharacterIndex(const Drawing::Point &position);
			
		private:
			UnicodeString text;
			
			Drawing::Size size;
			
			Drawing::IFont *font;
		};
	}
}

#endif