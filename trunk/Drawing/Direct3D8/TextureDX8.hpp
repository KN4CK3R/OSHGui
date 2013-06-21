/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_TEXTUREDX8_HPP
#define OSHGUI_DRAWING_TEXTUREDX8_HPP

#define NOMINMAX
#include <d3dx8.h>
#include <vector>
#include "../ITexture.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class RendererDX8;

		class OSHGUI_EXPORT TextureDX8 : public ITexture
		{
		public:
			using ITexture::Fill;
			using ITexture::FillGradient;

			/**
			 * Konstruktor der Klasse.
			 *
			 * @param renderer Zeiger auf den Parent RendererDX8
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice8-Objekt.
			 * @param size die Größe
			 * @param frameCount die Anzahl der Frames (default: 1)
			 * @param frameChangeInterval das Interval, in dem sich ein Frame ändert (default: 125ms)
			 */
			TextureDX8(RendererDX8 *renderer, IDirect3DDevice8 *device, const Size &size, int frameCount = 1, const Misc::TimeSpan &frameChangeInterval = Misc::TimeSpan::FromMilliseconds(125));
			/**
			 * Konstruktor der Klasse.
			 *
			 * @param renderer Zeiger auf den Parent RendererDX8
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice8-Objekt.
			 * @param filename Pfad zur zu ladenden Datei
			 */
			TextureDX8(RendererDX8 *renderer, IDirect3DDevice8 *device, const Misc::AnsiString &filename);
			virtual ~TextureDX8();
		
			/**
			 * Ruft das zugrunde liegende IDirect3DTexture9-Objekt ab.
			 *
			 * @return texture
			 */
			virtual IDirect3DTexture8* GetTexture() const;
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
			 * Muss einmalig vor dem Aufrufen von Clear, Fill oder FillGradient aufgerufen werden.
			 */
			virtual void BeginUpdate();
			/**
			 * Entsperrt die Textur.
			 * Muss einmalig nach dem Aufrufen von Clear, Fill oder FillGradient aufgerufen werden.
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
			
			virtual void PreReset();
			
			virtual void PostReset();

		private:
			void Create(const Size &size, int frameCount = 1);

			void ClearInternalData();

			RendererDX8 *renderer;
			IDirect3DDevice8 *device;

			Size realSize;
			
			D3DLOCKED_RECT lock;
			IDirect3DTexture8 *texture;
			std::vector<std::vector<Color> > framesData;

			int frame;
			std::vector<IDirect3DTexture8*> frames;
		};
	}
}

#endif