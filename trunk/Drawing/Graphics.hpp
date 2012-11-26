#ifndef OSHGUI_GRAPHICS_HPP
#define OSHGUI_GRAPHICS_HPP

#include "ITexture.hpp"
#include <vector>

namespace OSHGui
{
	namespace Drawing
	{
		class OSHGUI_EXPORT Graphics
		{
		public:
			Graphics(std::shared_ptr<ITexture> &texture);
			~Graphics();

			/**
			 * Löscht den Inhalt der Textur.
			 */
			void Clear();
			/**
			 * Zeichnet einen Pixel.
			 *
			 * @param origin der Punkt
			 * @param color die Farbe
			 */
			void SetPixel(const Point &origin, Color color);
			/**
			 * Zeichnet einen Pixel.
			 *
			 * @param x
			 * @param y
			 * @param color die Farbe
			 */
			void SetPixel(int x, int y, Color color);
			/**
			 * Füllt das Rechteck.
			 *
			 * @param origin
			 * @param size
			 * @param color die Farbe
			 */
			void FillRectangle(const Point &origin, const Size &size, Color color);
			/**
			 * Füllt das Rechteck.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 * @param color die Farbe
			 */
			void FillRectangle(int x, int y, int w, int h, Color color);
			/**
			 * Füllt die Textur mit einem Farbverlauf.
			 *
			 * @param origin
			 * @param size
			 * @param from die Startfarbe
			 * @param to die Endfarbe
			 * @param updown (optional: die Richtung des Farbverlaufs. Standard: von oben nach unten)
			 */
			void FillRectangleGradient(const Point &origin, const Size &size, Color from, Color to, bool updown = true);
			/**
			 * Füllt die Textur mit einem Farbverlauf.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 * @param from die Startfarbe
			 * @param to die Endfarbe
			 * @param updown (optional: die Richtung des Farbverlaufs. Standard: von oben nach unten)
			 */
			void FillRectangleGradient(int x, int y, int w, int h, Color from, Color to, bool updown = true);
			/**
			 * Füllt den Bereich mit der Farbe, der zwischen den Ecken des Polygons liegt.
			 *
			 * @param vertices
			 * @param color die Farbe
			 */
			void FillPolygon(const std::vector<Point> &vertices, Color color);
			/**
			 * Füllt einen Kreis.
			 *
			 * @param origin
			 * @param radius
			 * @param color die Farbe
			 */
			void FillCircle(const Point &origin, int radius, Color color);
			/**
			 * Füllt einen Kreis.
			 *
			 * @param x
			 * @param y
			 * @param radius
			 * @param color die Farbe
			 */
			void FillCircle(int x, int y, int radius, Color color);
			/**
			 * Füllt eine Ellipse mit der entsprechenden Größe um den Mittelpunkt herum.
			 *
			 * @param origin
			 * @param size
			 * @param color die Farbe
			 */
			void FillEllipse(const Point &origin, const Size &size, Color color);
			/**
			 * Füllt eine Ellipse mit der entsprechenden Größe um den Mittelpunkt herum.
			 *
			 * @param centerX
			 * @param centerY
			 * @param width
			 * @param height
			 * @param color die Farbe
			 */
			void FillEllipse(int centerX, int centerY, int width, int height, Color color);

		private:
			std::shared_ptr<ITexture> texture;
		};
	}
}

#endif