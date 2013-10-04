/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_TEXTUREOPENGL_HPP
#define OSHGUI_DRAWING_TEXTUREOPENGL_HPP

#include <vector>
#include "../ITexture.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class OSHGUI_EXPORT TextureOpenGL : public ITexture
		{
		public:
			using ITexture::Fill;
			using ITexture::FillGradient;

			/**
			 * Konstruktor der Klasse.
			 *
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 * @param size die Größe
			 * @param frameCount die Anzahl der Frames (default: 1)
			 * @param frameChangeInterval das Interval, in dem sich ein Frame ändert (default: 125ms)
			 */
			TextureOpenGL(const Size &size, int frameCount = 1, Misc::TimeSpan frameChangeInterval = Misc::TimeSpan::FromMilliseconds(125));
			/**
			 * Konstruktor der Klasse.
			 *
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 * @param filename Pfad zur zu ladenden Datei
			 */
			TextureOpenGL(const Misc::AnsiString &filename);
			virtual ~TextureOpenGL();
			
			/**
			 * Ruft die Textur ab.
			 */
			unsigned int GetTexture() const;

			/**
			 * Ruft ab, ob die Textur gesperrt ist.
			 */
			virtual bool IsLocked() const;

			/**
			 * Lädt eine Textur aus einer Datei.
			 *
			 * @param filename der Dateipfad
			 * @return gibt den Status der Operation zurück
			 */
			virtual void LoadFromFile(const Misc::AnsiString &filename);

			/**
			 * Sperrt die Textur, damit sie verändert werden kann.
			 * Muss einmalig vor dem Aufrufen von Clear, Fill, FillGradient, Rotate oder Insert aufgerufen werden.
			 */
			virtual void BeginUpdate();
			/**
			 * Entsperrt die Textur.
			 * Muss einmalig nach dem Aufrufen von Clear, Fill, FillGradient, Rotate oder Insert aufgerufen werden.
			 */
			virtual void EndUpdate();
			
			/**
			 * Füllt das Rechteck.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 * @param color die Farbe
			 */
			virtual void Fill(int x, int y, int w, int h, Color color);

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
			virtual void FillGradient(int x, int y, int w, int h, Color from, Color to, bool updown = true);

			//Animation
			/**
			 * Ruft ab, wieviele Frames die Textur beinhaltet.
			 *
			 * @return Anzahl der Frames
			 */
			virtual int GetFrameCount() const;
			/**
			 * Legt den Frame fest, auf den Änderungsmethoden angewandt werden.
			 *
			 * @param frame der Frame Index zwischen 0 und GetFrameCount()
			 */
			virtual void SelectActiveFrame(int frame);

		private:
			void Create(const Size &size, int frameCount = 1);
			
			int frame;
			std::vector<unsigned int> frames;

			unsigned int texture;
			unsigned int frameBuffer;
		};
	}
}

#endif