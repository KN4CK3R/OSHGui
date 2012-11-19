/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_ITEXTURE_HPP
#define OSHGUI_DRAWING_ITEXTURE_HPP

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
			virtual ~ITexture();

			/**
			 * Ruft die Größe der Textur ab.
			 *
			 * @return size
			 */
			const Size& GetSize() const;
			/**
			 * Ruft ab, ob die Textur gesperrt ist.
			 */
			virtual bool IsLocked() const = 0;

			/**
			 * Lädt eine Textur aus einer Datei.
			 *
			 * @param filename der Dateipfad
			 * @return gibt den Status der Operation zurück
			 */
			virtual void LoadFromFile(const Misc::AnsiString &filename) = 0;
		
			/**
			 * Sperrt die Textur, damit sie verändert werden kann.
			 * Muss einmalig vor dem Aufrufen von Clear, Fill oder FillGradient aufgerufen werden.
			 */
			virtual void BeginUpdate() = 0;
			/**
			 * Entsperrt die Textur.
			 * Muss einmalig nach dem Aufrufen von Clear, Fill oder FillGradient aufgerufen werden.
			 */
			virtual void EndUpdate() = 0;
			
			/**
			 * Löscht den Inhalt der Textur.
			 */
			virtual void Clear();
			/**
			 * Löscht einen 1x1 Pixel an der entsprechenden Position.
			 *
			 * @param point die Position
			 */
			virtual void Clear(const Point &point);
			/**
			 * Löscht einen 1x1 Pixel an der entsprechenden Position.
			 *
			 * @param x
			 * @param y
			 */
			virtual void Clear(int x, int y);
			/**
			 * Löscht das Rechteck.
			 *
			 * @param rect das Rechteck
			 */
			virtual void Clear(const Rectangle &rect);
			/**
			 * Löscht das Rechteck.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 */
			virtual void Clear(int x, int y, int w, int h);

			/**
			 * Füllt die Textur mit der angegebenen Farbe.
			 *
			 * @param color die Farbe
			 */
			virtual void Fill(Color color);
			/**
			 * Zeichnet ein 1x1 Pixel am entsprechenden Punkt.
			 *
			 * @param point der Punkt
			 * @param color die Farbe
			 */
			virtual void Fill(const Point &point, Color color);
			/**
			 * Zeichnet ein 1x1 Pixel am entsprechenden Punkt.
			 *
			 * @param x
			 * @param y
			 * @param color die Farbe
			 */
			virtual void Fill(int x, int y, Color color);
			/**
			 * Füllt das Rechteck.
			 *
			 * @param rect das Rechteck
			 * @param color die Farbe
			 */
			virtual void Fill(const Rectangle &rect, Color color);
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
			virtual void FillGradient(Color from, Color to, bool updown = true);
			/**
			 * Füllt das Rechteck mit einem Farbverlauf.
			 *
			 * @param rect das Rechteckt
			 * @param from die Startfarbe
			 * @param to die Endfarbe
			 * @param updown (optional: die Richtung des Farbverlaufs. Standard: von oben nach unten)
			 */
			virtual void FillGradient(const Rectangle &rect, Color from, Color to, bool updown = true);
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
			virtual const Misc::TimeSpan& GetFrameChangeInterval() const;
			/**
			 * Legt den Frame fest, auf den Änderungsmethoden angewandt werden.
			 *
			 * @param frame der Frame Index zwischen 0 und GetFrameCount()
			 */
			virtual void SelectActiveFrame(int frame) = 0;

		protected:		
			Size size;
			Misc::TimeSpan frameChangeInterval;
		};
	}
}

#endif