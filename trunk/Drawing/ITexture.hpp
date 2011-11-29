#ifndef OSHGUI_DRAWING_ITEXTURE_HPP_
#define OSHGUI_DRAWING_ITEXTURE_HPP_

#include <memory>
#include "../Exports.hpp"
#include "../Misc/Strings.hpp"
#include "../Misc/TimeSpan.hpp"
#include "Color.hpp"
#include "Point.hpp"
#include "Size.hpp"
#include "Rectangle.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Abstrakte Klasse, die eine Textur repräsentiert.
		 */
		class OSHGUI_EXPORT ITexture
		{
		public:
			/**
			 * Ruft die Größe der Textur ab.
			 *
			 * @return size
			 */
			const Size& GetSize() const { return size; }
			/**
			 * Ruft ab, ob die Textur gesperrt ist.
			 */
			virtual bool IsLocked() const = 0;
		
			/**
			 * Sperrt die Textur, damit sie verändert werden kann.
			 * Muss einmalig vor dem Aufrufen von Clear, Fill, FillGradient, Rotate oder Insert aufgerufen werden.
			 */
			virtual void BeginUpdate() = 0;
			/**
			 * Entsperrt die Textur.
			 * Muss einmalig nach dem Aufrufen von Clear, Fill, FillGradient, Rotate oder Insert aufgerufen werden.
			 */
			virtual void EndUpdate() = 0;
			
			/**
			 * Löscht den Inhalt der Textur.
			 */
			virtual void Clear() = 0;
			/**
			 * Löscht einen 1x1 Pixel an der entsprechenden Position.
			 *
			 * @param point die Position
			 */
			virtual void Clear(const Point &point) = 0;
			/**
			 * Löscht einen 1x1 Pixel an der entsprechenden Position.
			 *
			 * @param x
			 * @param y
			 */
			virtual void Clear(int x, int y) = 0;
			/**
			 * Löscht das Rechteck.
			 *
			 * @param rect das Rechteck
			 */
			virtual void Clear(const Rectangle &rect) = 0;
			/**
			 * Löscht das Rechteck.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 */
			virtual void Clear(int x, int y, int w, int h) = 0;

			/**
			 * Füllt die Textur mit der angegebenen Farbe.
			 *
			 * @param color die Farbe
			 */
			virtual void Fill(Color color) = 0;
			/**
			 * Zeichnet ein 1x1 Pixel am entsprechenden Punkt.
			 *
			 * @param point der Punkt
			 * @param color die Farbe
			 */
			virtual void Fill(const Point &point, Color color) = 0;
			/**
			 * Zeichnet ein 1x1 Pixel am entsprechenden Punkt.
			 *
			 * @param x
			 * @param y
			 * @param color die Farbe
			 */
			virtual void Fill(int x, int y, Color color) = 0;
			/**
			 * Füllt das Rechteck.
			 *
			 * @param rect das Rechteck
			 * @param color die Farbe
			 */
			virtual void Fill(const Rectangle &rect, Color color) = 0;
			/**
			 * Füllt das Rechteck.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 * @param color die Farbe
			 */
			virtual void Fill(int x, int y, int w, int h, Color color) = 0;

			/**
			 * Füllt die Textur mit einem Farbverlauf.
			 *
			 * @param from die Startfarbe
			 * @param to die Endfarbe
			 * @param updown (optional: die Richtung des Farbverlaufs. Standard: von oben nach unten)
			 */
			virtual void FillGradient(Color from, Color to, bool updown = true) = 0;
			/**
			 * Füllt das Rechteck mit einem Farbverlauf.
			 *
			 * @param rect das Rechteckt
			 * @param from die Startfarbe
			 * @param to die Endfarbe
			 * @param updown (optional: die Richtung des Farbverlaufs. Standard: von oben nach unten)
			 */
			virtual void FillGradient(const Rectangle &rect, Color from, Color to, bool updown = true) = 0;
			/**
			 * Füllt das Rechteck mit einem Farbverlauf.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 * @param from die Startfarbe
			 * @param to die Endfarbe
			 * @param updown (optional: die Richtung des Farbverlaufs. Standard: von oben nach unten)
			 */
			virtual void FillGradient(int x, int y, int w, int h, Color from, Color to, bool updown = true) = 0;
			
			/**
			 * Dreht die Textur um die angegebene Gradzahl.
			 * Achtung: Bei einer Gradzahl != x*90° verändert sich die Größe der Textur!
			 *
			 * @param degrees die Gradzahl 0-360
			 */
			virtual void Rotate(int degrees) = 0;

			/**
			 * Fügt eine Textur am angegebenen Punkt in diese Textur ein.
			 *
			 * @param point der Ursprung
			 * @param texture die Textur
			 */
			virtual void Insert(const Point &point, const std::shared_ptr<ITexture> &texture) = 0;
			/**
			 * Fügt eine Textur am angegebenen Punkt in diese Textur ein.
			 *
			 * @param x
			 * @param y
			 * @param texture die Textur
			 */
			virtual void Insert(int x, int y, const std::shared_ptr<ITexture> &texture) = 0;

			//Animation
			/**
			 * Ruft ab, wieviele Frames die Textur beinhaltet.
			 *
			 * @return Anzahl der Frames
			 */
			virtual int GetFrameCount() const = 0;
			/**
			 * Ruft das FrameChangeInterval ab.
			 *
			 * @return frameChangeInterval
			 */
			virtual const Misc::TimeSpan& GetFrameChangeInterval() const = 0;
			virtual void AddFrame(const std::shared_ptr<ITexture> &frame) = 0;
			/**
			 * Legt den Frame fest, auf den Änderungsmethoden angewandt werden.
			 *
			 * @param frame der Frame Index zwischen 0 und GetFrameCount()
			 */
			virtual void SelectActiveFrame(int frame) = 0;

		protected:
			/**
			 * Erzeugt intern eine neue Textur mit der entsprechenden Größe.
			 *
			 * @param size die Größe
			 * @param frameCount die Anzahl der Frames
			 * @return gibt den Status der Operation zurück
			 */
			virtual void Create(const Size &size, int frameCount = 1) = 0;
			/**
			 * Lädt eine Textur aus einer Datei.
			 *
			 * @param filename der Dateipfad
			 * @return gibt den Status der Operation zurück
			 */
			virtual void LoadFromFile(const Misc::AnsiString &filename) = 0;
		
			Size size;
		};
	}
}

#endif