#ifndef OSHGUI_MISC_TEXTHELPER_HPP_
#define OSHGUI_MISC_TEXTHELPER_HPP_

#include <memory>
#include "../Exports.hpp"
#include "Misc\Strings.hpp"
#include "Drawing\IFont.hpp"
#include "Drawing\Point.hpp"
#include "Drawing\Size.hpp"

namespace OSHGui
{
	namespace Misc
	{
		/**
		 * Verwaltet eine Zeichenkette und deren Ausma�e.
		 */
		class OSHGUI_EXPORT TextHelper
		{
		public:
			/**
			 * Erstellt ein TextHelper-Objekt mit der entsprechenden Schriftart.
			 *
			 * @param font
			 */
			TextHelper(const std::shared_ptr<Drawing::IFont> &font);
			
			/**
			 * Legt die Schriftart fest.
			 *
			 * @param font darf nicht 0 sein
			 */
			void SetFont(const std::shared_ptr<Drawing::IFont> &font);
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
			 * @param index die Stelle ab der gel�scht werden soll
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
			Drawing::Point GetCharacterPosition(int index, bool trailing = false) const;
			/**
			 * Gibt die Weite eines Teilstrings zur�ck.
			 *
			 * @param index der Startindex
			 * @param size die L�nge der Zeichenkette. -1 = bis zum Ende (default: -1)
			 * @return die Gr��e
			 */
			Drawing::Size GetStringWidth(int index, int size = -1) const;
			/**
			 * Gibt den Index des Zeichens zur�ck, das der Position am n�chsten steht.
			 *
			 * @param position
			 * @return index
			 */
			int GetClosestCharacterIndex(const Drawing::Point &position) const;
			
		private:
			UnicodeString text;
			
			Drawing::Size size;
			
			std::shared_ptr<Drawing::IFont> font;
		};
	}
}

#endif