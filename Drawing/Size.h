#ifndef __OSHGUI_DRAWING_SIZE_H__
#define __OSHGUI_DRAWING_SIZE_H__

#include "Point.h"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Speichert ein geordnetes Paar von ganzen Zahlen, i. d. R. die Breite
		 * und H�he eines Rechtecks.
		 */
		class Size
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
			
			bool operator == (const Size &equal);
			bool operator != (const Size &equal);
			const Size operator - (const Size &s);
			Size& operator -= (const Size &s);
			const Size operator + (const Size &s);
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
			Size InflateEx(int width, int height);
			
			bool Contains(const Point &point);
			
			int Width,
				Height;
		};
	}
}

#endif