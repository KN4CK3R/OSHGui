#ifndef __OSHGUI_DRAWING_RENDERERDX9_H__
#define __OSHGUI_DRAWING_RENDERERDX9_H__

#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "IRenderer.h"
#include "TextureDX9.h"
#include "FontDX9.h"

namespace OSHGui
{
	namespace Drawing
	{
		struct Vertex2D
		{
			float x, y, z, rhw;
			D3DCOLOR color;
			float u, v;
		};
	
		class RendererDX9 : public IRenderer
		{
		public:
			RendererDX9(IDirect3DDevice9 *device);
			virtual ~RendererDX9();
			
			virtual void Begin();
			virtual void End();

			virtual std::shared_ptr<ITexture> CreateNewTexture();
			virtual std::shared_ptr<IFont> CreateNewFont();

			virtual void SetRenderRectangle(Rectangle &rect);
			
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, const Point &point);
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y);
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, const Rectangle &rect);
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y, int w, int h);

			virtual Size MeasureText(const std::shared_ptr<IFont> &font, const Misc::UnicodeString &text);

			virtual void RenderText(const std::shared_ptr<IFont> &font, const Point &point, const Misc::UnicodeString &text);
			virtual void RenderText(const std::shared_ptr<IFont> &font, int x, int y, const Misc::UnicodeString &text);
			virtual void RenderText(const std::shared_ptr<IFont> &font, Rectangle &rect, const Misc::UnicodeString &text);
			virtual void RenderText(const std::shared_ptr<IFont> &font, int x, int y, int w, int h, const Misc::UnicodeString &text);

			virtual void Fill(const Point &point);
			virtual void Fill(int x, int y);
			virtual void Fill(Rectangle &rect);
			virtual void Fill(int x, int y, int w, int h);

			virtual void FillGradient(const Point &point, Color &to);
			virtual void FillGradient(int x, int y, Color &to);
			virtual void FillGradient(Rectangle &rect, Color &to);
			virtual void FillGradient(int x, int y, int w, int h, Color &to);
			
		protected:
			void Flush();
			void AddVertex(int x, int y);
			void AddVertex(int x, int y, float u, float v);
		
			IDirect3DDevice9 *device;
			LPD3DXSPRITE sprite;

			std::shared_ptr<TextureDX9> texture;
			
			static const int maxVertices = 1024;
			int verticesNum;
			Vertex2D vertices[maxVertices];
			
			DWORD oldFVF;
		};
	}
}

#endif