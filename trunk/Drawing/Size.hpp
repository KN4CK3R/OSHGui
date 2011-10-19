#ifndef OSHGUI_DRAWING_SIZE_HPP_
#define OSHGUI_DRAWING_SIZE_HPP_

#include "../Exports.hpp"
#include "Point.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Speichert ein geordnetes Paar von ganzen Zahlen, i. d. R. die Breite
		 * und H�he eines Rechtecks.
		 */
		class OSHGUI_EXPORT Size
		{
		public:
			/**
			 * Erstellt ein Size-Objekt ohne Ausma�e.
			 */
			Size();
			/**
			 * Erstellt ein Size-Objekt mit der angegeben Breite und H�he
			 *
			 * @param width
			 * @param height
			 */
			Size(int width, int height);
			
			bool operator == (const Size &equal) const;
			bool operator != (const Size &equal) const;
			const Size operator - (const Size &s) const;
			Size& operator -= (const Size &s);
			const Size operator + (const Size &s) const;
			Size& operator += (const Size &s);
			
			/**
			 * Erweitert das Size-Objekt um die angegebe Breite und H�he.
			 *
			 * @param width
			 * @param height
			 */
			void Inflate(int width, int height);
			/**
			 * Kopiert das Size-Objekt und erweitert es um die angegebe Breite und H�he.
			 *
			 * @param width
			 * @param height
			 * @return size
			 */
			Size InflateEx(int width, int height) const;
			
			int Width,
				Height;
		};
	}
}

#endif