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
		class OSHGUI_EXPORT TextureDX9 : public ITexture
		{
		public:
			using ITexture::Fill;
			using ITexture::FillGradient;
			using ITexture::Insert;

			/**
			 * Konstruktor der Klasse.
			 *
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 * @param size die Größe
			 * @param frameCount die Anzahl der Frames (default: 1)
			 * @param frameChangeInterval das Interval, in dem sich ein Frame ändert (default: 125ms)
			 */
			TextureDX9(IDirect3DDevice9 *device, const Size &size, int frameCount = 1, Misc::TimeSpan frameChangeInterval = Misc::TimeSpan::FromMilliseconds(125));
			/**
			 * Konstruktor der Klasse.
			 *
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 * @param width
			 * @param height
			 * @param frameCount die Anzahl der Frames (default: 1)
			 * @param frameChangeInterval das Interval, in dem sich ein Frame ändert (default: 125ms)
			 */
			TextureDX9(IDirect3DDevice9 *device, int width, int height, int frameCount = 1, Misc::TimeSpan frameChangeInterval = Misc::TimeSpan::FromMilliseconds(125));
			/**
			 * Konstruktor der Klasse.
			 *
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 * @param filename Pfad zur zu ladenden Datei
			 */
			TextureDX9(IDirect3DDevice9 *device, const Misc::AnsiString &filename);
			virtual ~TextureDX9();
		
			/**
			 * Ruft das zugrunde liegende IDirect3DTexture9-Objekt ab.
			 *
			 * @return texture
			 */
			IDirect3DTexture9* GetTexture() const;
			/**
			 * Ruft ab, ob die Textur gesperrt ist.
			 */
			virtual bool IsLocked() const;
		
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
			
			/**
			 * Dreht die Textur um die angegebene Gradzahl.
			 * Achtung: Bei einer Gradzahl != x*90° verändert sich die Größe der Textur!
			 *
			 * @param degrees die Gradzahl 0-360
			 */
			virtual void Rotate(int degrees);

			/**
			 * Fügt eine Textur am angegebenen Punkt in diese Textur ein.
			 *
			 * @param x
			 * @param y
			 * @param texture die Textur
			 */
			virtual void Insert(int x, int y, const std::shared_ptr<ITexture> &texture);

			//Animation
			/**
			 * Ruft ab, wieviele Frames die Textur beinhaltet.
			 *
			 * @return Anzahl der Frames
			 */
			virtual int GetFrameCount() const;
			/**
			 * Fügt einen neuen Frame hinzu.
			 *
			 * @param frame der Frame
			 */
			virtual void AddFrame(const std::shared_ptr<ITexture> &frame);
			/**
			 * Legt den Frame fest, auf den Änderungsmethoden angewandt werden.
			 *
			 * @param frame der Frame Index zwischen 0 und GetFrameCount()
			 */
			virtual void SelectActiveFrame(int frame);
			
			void PreReset();
			
			void PostReset();
			
		protected:
			/**
			 * Erzeugt intern eine neue Textur mit der entsprechenden Größe und der Anzahl der Frames.
			 *
			 * @param size die Größe
			 * @param frameCount die Anzahl der Frames
			 * @return gibt den Status der Operation zurück
			 */
			virtual void Create(const Size &size, int frameCount = 1);
			/**
			 * Lädt eine Textur aus einer Datei.
			 *
			 * @param filename der Dateipfad
			 * @return gibt den Status der Operation zurück
			 */
			virtual void LoadFromFile(const Misc::AnsiString &filename);

		private:
			IDirect3DDevice9 *device;
			
			D3DLOCKED_RECT lock;
			
			IDirect3DTexture9 *texture;

			//Animation
			std::vector<IDirect3DTexture9*> frames;
			int frame;
			
			std::vector<std::vector<unsigned char> > framesReset;

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