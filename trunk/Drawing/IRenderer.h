#ifndef OSHGUI_DRAWING_IRENDERER_H_
#define OSHGUI_DRAWING_IRENDERER_H_

#include <memory>
#include "Color.h"
#include "Point.h"
#include "Size.h"
#include "Rectangle.h"
#include "ITexture.h"
#include "IFont.h"
#include "..\Misc\TimeSpan.h"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Abstrakte Klasse, die Zeichenmethoden bereitstellt.
		 */
		class IRenderer
		{
		public:		
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
			virtual const std::shared_ptr<ITexture> CreateNewTexture(const Misc::UnicodeString &filename) = 0;
			/**
			 * Erzeugt eine neue Schriftart.
			 *
			 * @return eine neue Schriftart
			 */
			virtual const std::shared_ptr<IFont> CreateNewFont(const Misc::UnicodeString &fontName, int size, bool bold, bool italic) = 0;
			
			/**
			 * Ruft die Standard-Schrift ab.
			 *
			 * @return die Standard-Schrift
			 */
			std::shared_ptr<IFont> GetDefaultFont() { return defaultFont; }
			
			/**
			 * Legt die Farbe zum Zeichnen fest.
			 *
			 * @param color die Farbe
			 */
			virtual void SetRenderColor(Color color) { this->color = color; }
			/**
			 * Ruft die Farbe zum Zeichnen ab.
			 *
			 * @return die Farbe
			 */
			virtual const Color& GetRenderColor() const { return color; }
			/**
			 * Legt das Rechteck fest, in dem gezeichnet wird.
			 *
			 * @param rect das Rechteck
			 */
			virtual void SetRenderRectangle(const Rectangle &rect) { renderRect = rect; }
			/**
			 * Ruft das Rechteck fest, in dem gezeichnet wird.
			 *
			 * @return das Rechteck
			 */
			virtual const Rectangle& GetRenderRectangle() const { return renderRect; }

			virtual const Size GetRenderDimension() const = 0;
			
			/**
			 * Zeichnet eine Textur am entsprechenden Punkt.
			 *
			 * @param texture die Textur
			 * @param point der Ursprung
			 */
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, const Point &point) = 0;
			/**
			 * Zeichnet eine Textur an deb entsprechenden Koordinaten.
			 *
			 * @param texture die Textur
			 * @param x
			 * @param y
			 */
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y) = 0;
			/**
			 * Zeichnet eine Textur im entsprechenden Rechteck. Die Textur wird bei Bedarf gestaucht.
			 *
			 * @param texture die Textur
			 * @param rect das Rechteck
			 */
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, const Rectangle &rect) = 0;
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
			virtual Size MeasureText(const std::shared_ptr<IFont> &font, const Misc::UnicodeString &text) = 0;

			/**
			 * Zeichnet einen Text am entsprechenden Punkt mit der entsprechenden Schriftart.
			 *
			 * @param font die Schriftart
			 * @param point der Ursprung
			 * @param text der Text
			 */
			virtual void RenderText(const std::shared_ptr<IFont> &font, const Point &point, const Misc::UnicodeString &text) = 0;
			/**
			 * Zeichnet einen Text am entsprechenden Punkt mit der entsprechenden Schriftart.
			 *
			 * @param font die Schriftart
			 * @param x
			 * @param y
			 * @param text der Text
			 */
			virtual void RenderText(const std::shared_ptr<IFont> &font, int x, int y, const Misc::UnicodeString &text) = 0;
			/**
			 * Zeichnet einen Text im entsprechenden Rechteck mit der entsprechenden Schriftart.
			 *
			 * @param font die Schriftart
			 * @param rect das Rechteck
			 * @param text der Text
			 */
			virtual void RenderText(const std::shared_ptr<IFont> &font, const Rectangle &rect, const Misc::UnicodeString &text) = 0;
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
			virtual void RenderText(const std::shared_ptr<IFont> &font, int x, int y, int w, int h, const Misc::UnicodeString &text) = 0;
			
			/**
			 * Zeichnet ein 1x1 Pixel am entsprechenden Punkt.
			 *
			 * @param point der Punkt
			 */
			virtual void Fill(const Point &point) = 0;
			/**
			 * Zeichnet ein 1x1 Pixel am entsprechenden Punkt.
			 *
			 * @param x
			 * @param y
			 */
			virtual void Fill(int x, int y) = 0;
			/**
			 * Füllt das Rechteck.
			 *
			 * @param rect das Rechteck
			 */
			virtual void Fill(const Rectangle &rect) = 0;
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
			virtual void FillGradient(const Rectangle &rect, Color &to) = 0;
			/**
			 * Füllt das Rechteck mit einem Farbverlauf.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 * @param to die Endfarbe
			 */
			virtual void FillGradient(int x, int y, int w, int h, Color &to) = 0;

		protected:
			Color color;
			Rectangle renderRect;
			std::shared_ptr<IFont> defaultFont;
		};
	}
}

#endif