/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_RENDERERDX9_HPP
#define OSHGUI_DRAWING_RENDERERDX9_HPP

#include <stdio.h>
#include <list>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "../IRenderer.hpp"
#include "TextureDX9.hpp"
#include "FontDX9.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Ein Render-Objekt, das f�r das Zeichnen mithilfe vom Direct3D zust�ndig ist.
		 */
		class OSHGUI_EXPORT RendererDX9 : public IRenderer
		{
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * @param device Zeiger auf ein initialisiertes IDirect3DDevice9 Objekt.
			 */
			RendererDX9(IDirect3DDevice9 *device);
			virtual ~RendererDX9();
			
			/**
			 * Leitet das Zeichnen ein.
			 */
			virtual void Begin();
			/**
			 * Schlie�t das Zeichnen ab.
			 */
			virtual void End();
			
			void PreReset();
			void PostReset();

			/**
			 * Erzeugt eine neue Textur.
			 *
			 * @return eine neue Textur
			 */
			virtual const std::shared_ptr<ITexture> CreateNewTexture(const Size &size, int frameCount = 1, Misc::TimeSpan frameChangeInterval = Misc::TimeSpan::FromMilliseconds(125));
			/**
			 * Erzeugt eine neue Textur.
			 *
			 * @return eine neue Textur
			 */
			virtual const std::shared_ptr<ITexture> CreateNewTexture(int width, int height, int frameCount = 1, Misc::TimeSpan frameChangeInterval = Misc::TimeSpan::FromMilliseconds(125));
			/**
			 * Erzeugt eine neue Textur.
			 *
			 * @return eine neue Textur
			 */
			virtual const std::shared_ptr<ITexture> CreateNewTexture(const Misc::AnsiString &filename);
			/**
			 * Erzeugt eine neue Schriftart.
			 *
			 * @return eine neue Schriftart
			 */
			virtual const std::shared_ptr<IFont> CreateNewFont(const Misc::AnsiString &fontName, int size, bool bold, bool italic);


			/**
			 * Legt das Rechteck fest, in dem gezeichnet wird.
			 *
			 * @param rect das Rechteck
			 */
			virtual void SetRenderRectangle(const Rectangle &rect);

			virtual const Size GetRenderDimension() const;
			
			/**
			 * Zeichnet eine Textur am entsprechenden Punkt.
			 *
			 * @param texture die Textur
			 * @param point der Ursprung
			 */
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, const Point &point);
			/**
			 * Zeichnet eine Textur an den entsprechenden Koordinaten.
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
			 * @param size die Gr��e
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
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y, int w, int h);

			/**
			 * Ruft die Ma�e des �bergebenen Texts unter Verwendung der entsprechenden Schriftart ab.
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
			 * @param point der Ursprung
			 * @param text der Text
			 */
			virtual void RenderText(const std::shared_ptr<IFont> &font, const Point &point, const Misc::AnsiString &text);
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
			 * @param size die Gr��e
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
			virtual void RenderText(const std::shared_ptr<IFont> &font, int x, int y, int w, int h, const Misc::AnsiString &text);

			/**
			 * Zeichnet ein 1x1 Pixel am entsprechenden Punkt.
			 *
			 * @param point der Punkt
			 */
			virtual void Fill(const Point &point);
			/**
			 * F�llt das Rechteck.
			 *
			 * @param location
			 * @param size
			 */
			virtual void Fill(const Point &location, const Size &size);
			/**
			 * Zeichnet ein 1x1 Pixel am entsprechenden Punkt.
			 *
			 * @param x
			 * @param y
			 */
			virtual void Fill(int x, int y);
			/**
			 * F�llt das Rechteck.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 */
			virtual void Fill(int x, int y, int w, int h);
			/**
			 * F�llt das Rechteck.
			 *
			 * @param rect das Rechteck
			 */
			virtual void Fill(const Rectangle &rect);

			/**
			 * F�llt das Rechteck mit einem Farbverlauf.
			 *
			 * @param point der Ursprung
			 * @param size die Gr��e
			 * @param to die Endfarbe
			 */
			virtual void FillGradient(const Point &point, const Size &size, const Color &to);
			/**
			 * F�llt das Rechteck mit einem Farbverlauf.
			 *
			 * @param rect das Rechteck
			 * @param to die Endfarbe
			 */
			virtual void FillGradient(const Rectangle &rect, const Color &to);
			/**
			 * F�llt das Rechteck mit einem Farbverlauf.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 * @param to die Endfarbe
			 */
			virtual void FillGradient(int x, int y, int w, int h, const Color &to);

		private:
			void Flush();
			void AddVertex(int x, int y);
			void AddVertex(int x, int y, float u, float v);
		
			IDirect3DDevice9 *device;

			std::shared_ptr<TextureDX9> texture;
			std::list<std::weak_ptr<TextureDX9> > textureList;
			std::list<std::weak_ptr<FontDX9> > fontList;
			
			static const int maxVertices = 1024;
			int verticesNum;
			
			struct Vertex2D
			{
				float x, y, z, rhw;
				D3DCOLOR color;
				float u, v;
			};
			
			Vertex2D vertices[maxVertices];
			
			IDirect3DStateBlock9 *stateBlock;
		};
	}
}

#endif