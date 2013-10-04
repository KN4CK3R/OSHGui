/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_SIZE_HPP
#define OSHGUI_DRAWING_SIZE_HPP

#include "../Exports.hpp"
#include "Point.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Speichert ein geordnetes Paar von ganzen Zahlen, i. d. R. die Breite
		 * und Höhe eines Rechtecks.
		 */
		class OSHGUI_EXPORT Size
		{
		public:
			/**
			 * Erstellt ein Size-Objekt ohne Ausmaße.
			 */
			Size();
			/**
			 * Erstellt ein Size-Objekt mit der angegeben Breite und Höhe
			 *
			 * @param width
			 * @param height
			 */
			Size(int width, int height);
			
			friend bool operator==(const Size &lhs, const Size &rhs);
			friend bool operator<(const Size &lhs, const Size &rhs);

			const Size operator - (const Size &s) const;
			Size& operator -= (const Size &s);
			const Size operator + (const Size &s) const;
			Size& operator += (const Size &s);
			
			/**
			 * Erweitert das Size-Objekt um die angegebe Breite und Höhe.
			 *
			 * @param width
			 * @param height
			 */
			void Inflate(int width, int height);
			/**
			 * Kopiert das Size-Objekt und erweitert es um die angegebe Breite und Höhe.
			 *
			 * @param width
			 * @param height
			 * @return size
			 */
			Size InflateEx(int width, int height) const;
			
			int Width,
				Height;
		};

		bool operator==(const Size &lhs, const Size &rhs);
		inline bool operator!=(const Size &lhs, const Size &rhs) { return !(lhs == rhs); }
		bool operator<(const Size &lhs, const Size &rhs);
		inline bool operator>(const Size &lhs, const Size &rhs) { return rhs < lhs; }
		inline bool operator>=(const Size &lhs, const Size &rhs) { return !(rhs < lhs); }
		inline bool operator<=(const Size &lhs, const Size &rhs) { return !(rhs > lhs); }
	}
}

#endif