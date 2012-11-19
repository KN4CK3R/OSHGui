/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_TEXTUREDX9_HPP
#define OSHGUI_DRAWING_TEXTUREDX9_HPP

#include <vector>
#define NOMINMAX
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "../ITexture.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class RendererDX9;

		class OSHGUI_EXPORT TextureDX9 : public ITexture
		{
		public:
			using ITexture::Fill;
			using ITexture::FillGradient;

			/**
			 * Konstruktor der Klasse.
			 *
			 * @param renderer Zeiger auf den Parent RendererDX9
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 * @param size die Größe
			 * @param frameCount die Anzahl der Frames (default: 1)
			 * @param frameChangeInterval das Interval, in dem sich ein Frame ändert (default: 125ms)
			 */
			TextureDX9(RendererDX9 *renderer, IDirect3DDevice9 *device, const Size &size, int frameCount = 1, const Misc::TimeSpan &frameChangeInterval = Misc::TimeSpan::FromMilliseconds(125));
			/**
			 * Konstruktor der Klasse.
			 *
			 * @param renderer Zeiger auf den Parent RendererDX9
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 * @param filename Pfad zur zu ladenden Datei
			 */
			TextureDX9(RendererDX9 *renderer, IDirect3DDevice9 *device, const Misc::AnsiString &filename);
			virtual ~TextureDX9();
		
			/**
			 * Ruft das zugrunde liegende IDirect3DTexture9-Objekt ab.
			 *
			 * @return texture
			 */
			virtual IDirect3DTexture9* GetTexture() const;
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
			 * Lädt eine Texture aus der Resource.
			 *
			 * @param module Modul mit der Resource
			 * @param name Name der Resource
			 */
			virtual void LoadFromWin32Resource(HMODULE module, LPCSTR name);
		
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

			RendererDX9 *renderer;
			IDirect3DDevice9 *device;

			Size realSize;
			
			D3DLOCKED_RECT lock;
			IDirect3DTexture9 *texture;
			std::vector<std::vector<Color> > framesData;

			int frame;
			std::vector<IDirect3DTexture9*> frames;

			class TextureDX9Iterator
			{
			public:
				TextureDX9Iterator(const D3DLOCKED_RECT &lock);

				void GoTo(int x, int y);
				void NextLine();
				void NextColumn();

				void SetColor(const Drawing::Color &color);
				const Drawing::Color GetColor() const;

			private:
				D3DLOCKED_RECT lock;
				int *data;
				int pitch;
				int position;
				int x;
				int y;
			};
		};
	}
}

#endif