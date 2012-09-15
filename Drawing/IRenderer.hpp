/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_IRENDERER_HPP
#define OSHGUI_DRAWING_IRENDERER_HPP

#include <memory>
#include "../Exports.hpp"
#include "Color.hpp"
#include "Point.hpp"
#include "Size.hpp"
#include "Rectangle.hpp"
#include "ITexture.hpp"
#include "IFont.hpp"
#include "../Misc/TimeSpan.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Abstrakte Klasse, die Zeichenmethoden bereitstellt.
		 */
		class OSHGUI_EXPORT IRenderer
		{
		public:
			virtual ~IRenderer();

			/**
			 * Leitet das Zeichnen ein.
			 */
			virtual void Begin() = 0;
			/**
			 * Schließt das Zeichnen ab.
			 */
			virtual void End() = 0;

			/**
			 * Erzeugt eine neue Textur mit der angebenen Größe, Anzahl der Frames und deren Wechselinterval.
			 *
			 * @param size die Größe
			 * @param frameCount Anzahl der Frames (optional für animierte Texturen)
			 * @param frameChangeInterval Interval der Framewechsel (optional für animierte Texturen)
			 * @return die Textur
			 */
			virtual const std::shared_ptr<ITexture> CreateNewTexture(const Size &size, int frameCount = 1, Misc::TimeSpan frameChangeInterval = Misc::TimeSpan::FromMilliseconds(125)) = 0;
			/**
			 * Erzeugt eine neue Textur mit der angebenen Größe, Anzahl der Frames und deren Wechselinterval.
			 *
			 * @param width die Breite
			 * @param height die Höhe
			 * @param frameCount Anzahl der Frames (optional für animierte Texturen)
			 * @param frameChangeInterval Interval der Framewechsel (optional für animierte Texturen)
			 * @return die Textur
			 */
			virtual const std::shared_ptr<ITexture> CreateNewTexture(int width, int height, int frameCount = 1, Misc::TimeSpan frameChangeInterval = Misc::TimeSpan::FromMilliseconds(125)) = 0;
			/**
			 * Lädt eine Textur aus einer Datei.
			 *
			 * @return die Textur
			 */
			virtual const std::shared_ptr<ITexture> CreateNewTexture(const Misc::AnsiString &filename) = 0;
			/**
			 * Erzeugt eine neue Schriftart.
			 *
			 * @return eine neue Schriftart
			 */
			virtual const std::shared_ptr<IFont> CreateNewFont(const Misc::AnsiString &fontName, int size, bool bold, bool italic) = 0;
			
			/**
			 * Ruft die Standard-Schrift ab.
			 *
			 * @return die Standard-Schrift
			 */
			std::shared_ptr<IFont> GetDefaultFont() const;
			
			/**
			 * Legt die Farbe zum Zeichnen fest.
			 *
			 * @param color die Farbe
			 */
			virtual void SetRenderColor(const Color &color);
			/**
			 * Ruft die Farbe zum Zeichnen ab.
			 *
			 * @return die Farbe
			 */
			virtual const Color& GetRenderColor() const;
			/**
			 * Legt das Rechteck fest, in dem gezeichnet wird.
			 *
			 * @param rect das Rechteck
			 */
			virtual void SetRenderRectangle(const Rectangle &rect);
			/**
			 * Ruft das Rechteck fest, in dem gezeichnet wird.
			 *
			 * @return das Rechteck
			 */
			virtual const Rectangle& GetRenderRectangle() const;
			/**
			 * Ruft die Größe des Zeichenbereichs ab.
			 *
			 * @return Größe des Zeichenbereichs
			 */
			virtual const Size GetRenderDimension() const = 0;
			
			/**
			 * Zeichnet eine Textur am entsprechenden Punkt.
			 *
			 * @param texture die Textur
			 * @param point der Ursprung
			 */
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, const Point &point);
			/**
			 * Zeichnet eine Textur an deb entsprechenden Koordinaten.
			 *
			 * @param texture die Textur
			 * @param x
			 * @param y
			 */
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y);
			/**
			 * Zeichnet eine Textur im entsprechenden Rechteck. Die Textur wird bei Bedarf gestaucht.
			 *
			 * @param texture die Textur
			 * @param point der Startpunkt
			 * @param size die Größe
			 */
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, const Point &point, const Size &size);
			/**
			 * Zeichnet eine Textur im entsprechenden Rechteck. Die Textur wird bei Bedarf gestaucht.
			 *
			 * @param texture die Textur
			 * @param rect das Rechteck
			 */
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, const Rectangle &rect);
			/**
			 * Zeichnet eine Textur im entsprechenden Rechteck. Die Textur wird bei Bedarf gestaucht.
			 *
			 * @param texture die Textur
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 */
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y, int w, int h) = 0;

			/**
			 * Ruft die Maße des übergebenen Texts unter Verwendung der entsprechenden Schriftart ab.
			 *
			 * @param font die Schriftart
			 * @param text der Text
			 * @return size
			 */
			virtual Size MeasureText(const std::shared_ptr<IFont> &font, const Misc::AnsiString &text);

			/**
			 * Zeichnet einen Text am entsprechenden Punkt mit der entsprechenden Schriftart.
			 *
			 * @param font die Schriftart
			 * @param location der Ursprung
			 * @param text der Text
			 */
			virtual void RenderText(const std::shared_ptr<IFont> &font, const Point &location, const Misc::AnsiString &text);
			/**
			 * Zeichnet einen Text am entsprechenden Punkt mit der entsprechenden Schriftart.
			 *
			 * @param font die Schriftart
			 * @param x
			 * @param y
			 * @param text der Text
			 */
			virtual void RenderText(const std::shared_ptr<IFont> &font, int x, int y, const Misc::AnsiString &text);
			/**
			 * Zeichnet einen Text im entsprechenden Rechteck mit der entsprechenden Schriftart.
			 *
			 * @param font die Schriftart
			 * @param location der Ursprung
			 * @param size die Größe
			 * @param text der Text
			 */
			virtual void RenderText(const std::shared_ptr<IFont> &font, const Point &location, const Size &size, const Misc::AnsiString &text);
			/**
			 * Zeichnet einen Text im entsprechenden Rechteck mit der entsprechenden Schriftart.
			 *
			 * @param font die Schriftart
			 * @param rect das Rechteck
			 * @param text der Text
			 */
			virtual void RenderText(const std::shared_ptr<IFont> &font, const Rectangle &rect, const Misc::AnsiString &text);
			/**
			 * Zeichnet einen Text im entsprechenden Rechteck mit der entsprechenden Schriftart.
			 *
			 * @param font die Schriftart
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 * @param text der Text
			 */
			virtual void RenderText(const std::shared_ptr<IFont> &font, int x, int y, int w, int h, const Misc::AnsiString &text) = 0;
			
			/**
			 * Zeichnet ein 1x1 Pixel am entsprechenden Punkt.
			 *
			 * @param point der Punkt
			 */
			virtual void Fill(const Point &point);
			/**
			 * Zeichnet ein 1x1 Pixel am entsprechenden Punkt.
			 *
			 * @param x
			 * @param y
			 */
			virtual void Fill(int x, int y);
			/**
			 * Füllt das Rechteck.
			 *
			 * @param location
			 * @param size
			 */
			virtual void Fill(const Point &location, const Size &size);
			/**
			 * Füllt das Rechteck.
			 *
			 * @param rect das Rechteck
			 */
			virtual void Fill(const Rectangle &rect);
			/**
			 * Füllt das Rechteck.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 */
			virtual void Fill(int x, int y, int w, int h) = 0;
			
			/**
			 * Füllt das Rechteck mit einem Farbverlauf.
			 *
			 * @param rect das Rechteck
			 * @param to die Endfarbe
			 */
			virtual void FillGradient(const Rectangle &rect, const Color &to);
			/**
			 * Füllt das Rechteck mit einem Farbverlauf.
			 *
			 * @param point der Ursprung
			 * @param size die Größe
			 * @param to die Endfarbe
			 */
			virtual void FillGradient(const Point &point, const Size &size, const Color &to);
			/**
			 * Füllt das Rechteck mit einem Farbverlauf.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 * @param to die Endfarbe
			 */
			virtual void FillGradient(int x, int y, int w, int h, const Color &to) = 0;

			/**
			 * Beginnt das Zeichnen von Linien. Bevor "normal" gezeichnet wird, muss EndLines aufgerufen werden.
			 */
			virtual void BeginLines() = 0;
			/**
			 * Zeichnet eine Linie von from nach to.
			 *
			 * @param from
			 * @param to
			 */
			virtual void RenderLine(const Point &from, const Point &to);
			/**
			 * Zeichnet eine Linie von (x1,y1) nach (x2,y2).
			 *
			 * @param x1
			 * @param y1
			 * @param x2
			 * @param y2
			 */
			virtual void RenderLine(int x1, int y1, int x2, int y2) = 0;
			/**
			 * Beendet das Zeichnen von Linien.
			 */
			virtual void EndLines() = 0;

		protected:
			Color color;
			Rectangle renderRect;
			std::shared_ptr<IFont> defaultFont;
		};
	}
}

#endif