#ifndef __OSHGUI_DRAWING_TEXTUREDX9_H__
#define __OSHGUI_DRAWING_TEXTUREDX9_H__

#include <vector>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "ITexture.h"

namespace OSHGui
{
	namespace Drawing
	{
		class TextureDX9 : public ITexture
		{
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 * @param size die Gr��e
			 * @param frameCount die Anzahl der Frames (default: 1)
			 */
			TextureDX9(IDirect3DDevice9 *device, const Size &size, int frameCount = 1);
			/**
			 * Konstruktor der Klasse.
			 *
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 * @param width
			 * @param height
			 * @param frameCount die Anzahl der Frames (default: 1)
			 */
			TextureDX9(IDirect3DDevice9 *device, int width, int height, int frameCount = 1);
			/**
			 * Konstruktor der Klasse.
			 *
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9-Objekt.
			 * @param size die Gr��e
			 * @param frameCount die Anzahl der Frames (default: 1)
			 */
			TextureDX9(IDirect3DDevice9 *device, const Misc::UnicodeString &filename);
			virtual ~TextureDX9();
		
			/**
			 * Ruft das zugrunde liegende IDirect3DTexture9-Objekt ab.
			 *
			 * @return texture
			 */
			IDirect3DTexture9* GetTexture();
			/**
			 * Ruft ab, ob die Textur gesperrt ist.
			 */
			virtual bool IsLocked();
		
			/**
			 * Sperrt die Textur, damit sie ver�ndert werden kann.
			 * Muss einmalig vor dem Aufrufen von Clear, Fill, FillGradient, Rotate oder Insert aufgerufen werden.
			 */
			virtual void BeginUpdate();
			/**
			 * Entsperrt die Textur.
			 * Muss einmalig nach dem Aufrufen von Clear, Fill, FillGradient, Rotate oder Insert aufgerufen werden.
			 */
			virtual void EndUpdate();
			
			/**
			 * L�scht den Inhalt der Textur.
			 */
			virtual void Clear();
			/**
			 * L�scht einen 1x1 Pixel an der entsprechenden Position.
			 *
			 * @param point die Position
			 */
			virtual void Clear(const Point &point);
			/**
			 * L�scht einen 1x1 Pixel an der entsprechenden Position.
			 *
			 * @param x
			 * @param y
			 */
			virtual void Clear(int x, int y);
			/**
			 * L�scht das Rechteck.
			 *
			 * @param rect das Rechteck
			 */
			virtual void Clear(const Rectangle &rect);
			/**
			 * L�scht das Rechteck.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 */
			virtual void Clear(int x, int y, int w, int h);

			/**
			 * F�llt die Textur mit der angegebenen Farbe.
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
			 * F�llt das Rechteck.
			 *
			 * @param rect das Rechteck
			 * @param color die Farbe
			 */
			virtual void Fill(const Rectangle &rect, Color color);
			/**
			 * F�llt das Rechteck.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 * @param color die Farbe
			 */
			virtual void Fill(int x, int y, int w, int h, Color color);

			/**
			 * F�llt die Textur mit einem Farbverlauf.
			 *
			 * @param from die Startfarbe
			 * @param to die Endfarbe
			 * @param updown (optional: die Richtung des Farbverlaufs. Standard: von oben nach unten)
			 */
			virtual void FillGradient(Color from, Color to, bool updown = true);
			/**
			 * F�llt das Rechteck mit einem Farbverlauf.
			 *
			 * @param rect das Rechteckt
			 * @param from die Startfarbe
			 * @param to die Endfarbe
			 * @param updown (optional: die Richtung des Farbverlaufs. Standard: von oben nach unten)
			 */
			virtual void FillGradient(const Rectangle &rect, Color from, Color to, bool updown = true);
			/**
			 * F�llt das Rechteck mit einem Farbverlauf.
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
			 * Achtung: Bei einer Gradzahl != x*90� ver�ndert sich die Gr��e der Textur!
			 *
			 * @param degrees die Gradzahl 0-360
			 */
			virtual void Rotate(int degrees);

			/**
			 * F�gt eine Textur am angegebenen Punkt in diese Textur ein.
			 *
			 * @param point der Ursprung
			 * @param texture die Textur
			 */
			virtual void Insert(const Point &point, const std::shared_ptr<ITexture> &texture);
			/**
			 * F�gt eine Textur am angegebenen Punkt in diese Textur ein.
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
			virtual int GetFrameCount() { return frames.size(); }
			virtual const Misc::TimeSpan& GetFrameChangeInterval() { return frameChangeInterval; }
			/**
			 * Legt den Frame fest, auf den �nderungsmethoden angewandt werden.
			 *
			 * @param frame der Frame Index zwischen 0 und GetFrameCount()
			 */
			virtual void SelectActiveFrame(int frame)
			{
				if (frame < 0 || frame >= GetFrameCount())
				{
					frame = 0;
				}

				texture = frames[frame];
			}
			
		protected:
			/**
			 * Erzeugt intern eine neue Textur mit der entsprechenden Gr��e.
			 *
			 * @param size die Gr��e
			 * @return gibt den Status der Operation zur�ck
			 */
			virtual bool Create(const Size &size, int frameCount = 1);
			/**
			 * L�dt eine Textur aus einer Datei.
			 *
			 * @param filename der Dateipfad
			 * @return gibt den Status der Operation zur�ck
			 */
			virtual bool LoadFromFile(const Misc::UnicodeString &filename);

		private:
			IDirect3DDevice9 *device;
			
			D3DLOCKED_RECT lock;
			
			IDirect3DTexture9 *texture;

			//Animation
			Misc::TimeSpan frameChangeInterval;
			std::vector<IDirect3DTexture9*> frames;
		};
	}
}

#endif