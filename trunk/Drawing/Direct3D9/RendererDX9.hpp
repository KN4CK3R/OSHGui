/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_RENDERERDX9_HPP
#define OSHGUI_DRAWING_RENDERERDX9_HPP

#include <list>

#include "../IRenderer.hpp"
#include "TextureDX9.hpp"
#include "FontDX9.hpp"

struct ID3DXSprite;
struct IDirect3DDevice9;
struct IDirect3DStateBlock9;

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Ein Render-Objekt, das für das Zeichnen mithilfe vom Direct3D zuständig ist.
		 */
		class OSHGUI_EXPORT RendererDX9 : public IRenderer
		{
		public:
			using IRenderer::Fill;
			using IRenderer::FillGradient;
			using IRenderer::RenderTexture;
			using IRenderer::RenderText;
			using IRenderer::RenderLine;

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
			 * Schließt das Zeichnen ab.
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
			 * Ruft die Größe des Zeichenbereichs ab.
			 *
			 * @return Größe des Zeichenbereichs
			 */
			virtual const Size GetRenderDimension() const;
			
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
			 * Füllt das Rechteck.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 */
			virtual void Fill(int x, int y, int w, int h);

			/**
			 * Füllt das Rechteck mit einem Farbverlauf.
			 *
			 * @param x
			 * @param y
			 * @param w
			 * @param h
			 * @param to die Endfarbe
			 */
			virtual void FillGradient(int x, int y, int w, int h, const Color &to);

			/**
			 * Beginnt das Zeichnen von Linien. Bevor "normal" gezeichnet wird, muss EndLines aufgerufen werden.
			 */
			virtual void BeginLines();
			/**
			 * Zeichnet eine Linie von (x1,y1) nach (x2,y2).
			 *
			 * @param x1
			 * @param y1
			 * @param x2
			 * @param y2
			 */
			virtual void RenderLine(int x1, int y1, int x2, int y2);
			/**
			 * Beendet das Zeichnen von Linien.
			 */
			virtual void EndLines();

		private:
			void InitializeDevice();
			void Flush();
			void AddVertex(int x, int y);
			void AddVertex(int x, int y, float u, float v);
		
			IDirect3DDevice9 *device;

			std::shared_ptr<TextureDX9> texture;
			std::vector<std::weak_ptr<TextureDX9> > textureList;
			std::vector<std::weak_ptr<FontDX9> > fontList;
			
			static const int maxVertices = 1024;
			int verticesNum;
			bool flushSprite;
			
			struct Vertex2D
			{
				float x;
				float y;
				float z;
				//float rhw;
				D3DCOLOR color;
				float u;
				float v;
			};
			
			Vertex2D vertices[maxVertices];
			
			IDirect3DStateBlock9 *stateBlockBackup;
			ID3DXSprite *sprite;
		};
	}
}

#endif