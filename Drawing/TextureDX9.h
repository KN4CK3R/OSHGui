#ifndef __OSHGUI_DRAWING_TEXTUREDX9_H__
#define __OSHGUI_DRAWING_TEXTUREDX9_H__

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
			 */
			TextureDX9(IDirect3DDevice9 *device);
			virtual ~TextureDX9();
		
			/**
			 * Ruft das zugrunde liegende IDirect3DTexture9-Objekt ab.
			 *
			 * @return texture
			 */
			LPDIRECT3DTEXTURE9 GetTexture();
			/**
			 * Ruft ab, ob die Textur gesperrt ist.
			 */
			virtual bool IsLocked();

			/**
			 * Erzeugt intern eine neue Textur mit der entsprechenden Größe.
			 *
			 * @param size die Größe
			 * @return gibt den Status der Operation zurück
			 */
			virtual bool Create(const Size &size);
			/**
			 * Erzeugt intern eine neue Textur mit der entsprechenden Größe.
			 *
			 * @param width
			 * @param height
			 * @return gibt den Status der Operation zurück
			 */
			virtual bool Create(int width, int height);
			/**
			 * Lädt eine Textur aus einer Datei.
			 *
			 * @param filename der Dateipfad
			 * @return gibt den Status der Operation zurück
			 */
			virtual bool LoadFromFile(const Misc::UnicodeString &filename);
		
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
			virtual void Fill(int x, int y, int w, int h, Color color);

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
			 * @param point der Ursprung
			 * @param texture die Textur
			 */
			virtual void Insert(const Point &point, ITexture *texture);
			/**
			 * Fügt eine Textur am angegebenen Punkt in diese Textur ein.
			 *
			 * @param x
			 * @param y
			 * @param texture die Textur
			 */
			virtual void Insert(int x, int y, ITexture *texture);
			
		private:
			IDirect3DDevice9 *device;
			
			D3DLOCKED_RECT lock;
			
			LPDIRECT3DTEXTURE9 texture;
		};
	}
}

#endif