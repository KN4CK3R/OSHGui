#ifndef __OSHGUI_DRAWING_RECTANGLE_H__
#define __OSHGUI_DRAWING_RECTANGLE_H__

#include "Point.h"
#include "Size.h"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Speichert einen Satz von vier ganzen Zahlen, die die Position und Gr��e
		 * eines Rechtecks angeben.
		 */
		class Rectangle
		{			
		public:
			/**
			 * Erstellt ein Rechteck ohne Ausma�e.
			 */
			Rectangle();
			/**
			 * Erstellt ein Rechteck an der angegeben Position mit der angegeben Gr��e.
			 *
			 * @param left
			 * @param top
			 * @param width
			 * @param height
			 */
			Rectangle(int left, int top, int width, int height);
			
			void SetLeft(int left);
			int GetLeft() const;
			void SetTop(int top);
			int GetTop() const;
			void SetWidth(int width);
			int GetWidth() const;
			void SetHeight(int height);
			int GetHeight() const;
			int GetRight() const;
			int GetBottom() const;
			/**
			 * Ruft die Position des Rechtecks ab.
			 *
			 * @return position
			 */
			Point GetPosition() const;
			/**
			 * Ruft die Gr��e des Rechtecks ab.
			 *
			 * @return size
			 */
			Size GetSize() const;
			
			bool operator == (const Rectangle &equal);
			bool operator != (const Rectangle &equal);
			const Rectangle operator + (const Rectangle& add) const;
			const Rectangle operator + (const Point& add) const;
			const Rectangle operator - (const Rectangle& add) const;
			const Rectangle operator - (const Point& add) const;
			
			/**
			 * Verschiebt das Rechteck um X/Y.
			 *
			 * @param x
			 * @param y
			 */
			void Offset(int left, int top);
			/**
			 * Kopiert das Rechteck und verschiebt ihn um X/Y.
			 *
			 * @param x
			 * @param y
			 * @return rectangle
			 */
			Rectangle OffsetEx(int left, int top);
			/**
			 * Erweitert das Rectangle-Objekt um die angegebe Breite und H�he.
			 *
			 * @param width
			 * @param height
			 */
			void Inflate(int width, int height);
			/**
			 * Kopiert das Rectangle-Objekt und erweitert es um die angegebe Breite und H�he.
			 *
			 * @param width
			 * @param height
			 * @return rectangle
			 */
			Rectangle InflateEx(int width, int height);
			
			/**
			 * Gibt zur�ck, ob der Punkt innerhalb des Rechtecks liegt.
			 *
			 * @param point
			 * @return ja/nein
			 */
			bool Contains(const Point &point);

		private:
			int left,
				top,
				width,
				height;
			int bottom,
				right;
		};
	}
}

#endif