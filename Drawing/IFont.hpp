/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_IFONT_HPP
#define OSHGUI_DRAWING_IFONT_HPP

#include "../Exports.hpp"
#include "Point.hpp"
#include "Size.hpp"
#include "Rectangle.hpp"
#include "../Misc/Strings.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Definiert ein bestimmtes abstraktes Format für Text, einschließlich der Attribute
		 * für Schriftart, Schriftgrad und Schriftschnitt.
		 */
		class OSHGUI_EXPORT IFont
		{
		public:
			virtual ~IFont() { }

			/**
			 * Ruft den Namen der Schriftart ab.
			 *
			 * @return der Name
			 */
			const Misc::AnsiString& GetName() const { return name; }
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
			virtual const Size MeasureText(const Misc::AnsiString &text) = 0;
			
		protected:
			/**
			 * Erzeugt eine neue Schriftart.
			 *
			 * @param fontName der Name der Schriftart
			 * @param size die Schriftgröße
			 * @param bold fettgedruckt
			 * @param italic kursiv
			 */
			virtual void Create(const Misc::AnsiString &name, int size, bool bold, bool italic) = 0;
		
			Misc::AnsiString name;
			int size;
			bool bold;
			bool italic;
		};
	}
}

#endif