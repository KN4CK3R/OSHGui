/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_IMAGEGRAPHICS_HPP
#define OSHGUI_IMAGEGRAPHICS_HPP

#include "Rectangle.hpp"
#include <vector>

namespace OSHGui
{
	namespace Drawing
	{
		class ColorRectangle;
		class CustomizableImage;

		class OSHGUI_EXPORT ImageGraphics
		{
		public:
			ImageGraphics(CustomizableImage &image);

			/**
			 * L�scht den Inhalt der Textur.
			 */
			void Clear();

			void DrawLine(const ColorRectangle &color, const PointF &from, const PointF &to);
			/**
			 * F�llt das Rechteck.
			 *
			 * \param color
			 * \param origin
			 * \param size
			 */
			void DrawRectangle(const ColorRectangle &color, const PointF &origin, const SizeF &size);
			/**
			 * F�llt das Rechteck.
			 *
			 * \param color
			 * \param rectangle
			 */
			void DrawRectangle(const ColorRectangle &color, const RectangleF &rectangle);
			/**
			 * F�llt die Textur mit einem Farbverlauf.
			 *
			 * \param colors die Eckfarben
			 * \param origin
			 * \param size
			 */
			void FillRectangle(const ColorRectangle &colors, const PointF &origin, const SizeF &size);
			/**
			 * F�llt die Textur mit einem Farbverlauf.
			 *
			 * \param colors die Eckfarben
			 * \param origin
			 * \param size
			 */
			void FillRectangle(const ColorRectangle &colors, const RectangleF &rectangle);
			/**
			 * F�llt den Bereich mit der Farbe, der zwischen den Ecken des Polygons liegt.
			 *
			 * \param vertices
			 */
			void FillPolygon(const std::vector<PointF> &vertices, const ColorRectangle &color);
			/**
			 * F�llt einen Kreis.
			 *
			 * \param color
			 * \param origin
			 * \param radius
			 */
			void FillCircle(const ColorRectangle &color, const PointF &origin, float radius);
			/**
			 * F�llt eine Ellipse mit der entsprechenden Gr��e um den Mittelpunkt herum.
			 *
			 * \param color
			 * \param origin
			 * \param size
			 */
			void FillEllipse(const ColorRectangle &color, const PointF &origin, const SizeF &size);

			void FillEllipse(const ColorRectangle &color, const RectangleF &region);

		private:
			CustomizableImage &image;
		};
	}
}

#endif