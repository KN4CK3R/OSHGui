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
		 * Verwaltet eine Zeichenkette und deren Ausma�e.
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
			 * H�ngt das Zeichen an den Text an.
			 *
			 * @param character
			 */
			void Append(const UnicodeChar character);
			/**
			 * H�ngt die Zeichenkette an den Text an.
			 *
			 * @param text
			 */
			void Append(const UnicodeString &text);
			/**
			 * F�gt das Zeichen an der entsprechenden Position im Text ein.
			 *
			 * @param position
			 * @param character
			 */
			void Insert(int position, const UnicodeChar character);
			/**
			 * F�gt die Zeichenkette an der entsprechenden Position im Text ein.
			 *
			 * @param position
			 * @param text
			 */
			void Insert(int position, const UnicodeString &text);
			/**
			 * L�scht den kompletten Text.
			 */
			void Clear();
			/**
			 * L�scht das Zeichen an der entsprechenden Stelle.
			 *
			 * @param length Anzahl der zu l�schenden Zeichen (default: 1)
			 */
			void Remove(int index, int length = 1);
			
			/**
			 * Ruft die L�nge der Zeichenkette ab.
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
			 * Ruft die Ma�e der Zeichenkette in Pixel ab.
			 *
			 * @return size
			 */
			const Drawing::Size& GetSize() const;
			
			/**
			 * Berechnet die Ma�e neu.
			 */
			void RefreshSize();
			
			/**
			 * Gibt die Position des Zeichens an der entsprechenden Stelle zur�ck.
			 *
			 * @param index
			 * @param trailing linker Rand des Zeichens / rechter Rand des Zeichens
			 * @return position
			 */
			Drawing::Point GetCharacterPosition(int index, bool trailing = false);
			/**
			 * Gibt die Weite eines Teilstrings zur�ck.
			 * 
			 * @param index der Startindex
			 * @param size die L�nge der Zeichenkette. -1 = bis zum Ende (default: -1)
			 * @return die Gr��e
			 */
			Drawing::Size GetStringWidth(int index, int size = -1);
			/**
			 * Gibt den Index des Zeichens zur�ck, das der Position am n�chsten steht.
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