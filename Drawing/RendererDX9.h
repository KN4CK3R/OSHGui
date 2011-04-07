#ifndef __OSHGUI_DRAWING_RENDERERDX9_H__
#define __OSHGUI_DRAWING_RENDERERDX9_H__

#include <d3d9.h>

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

			virtual ITexture* CreateNewTexture();
			virtual IFont* CreateNewFont();
			
			virtual void RenderTexture(Drawing::ITexture *texture, const Drawing::Point &point);
			virtual void RenderTexture(Drawing::ITexture *texture, int x, int y);

			virtual Drawing::Size MeasureText(Drawing::IFont *font, LPCSTR text);
			virtual Drawing::Size MeasureTextEx(Drawing::IFont *font, LPCSTR text, ...);

			virtual void RenderText(Drawing::IFont *font, const Drawing::Point &point, LPCSTR text);
			virtual void RenderText(Drawing::IFont *font, int x, int y, LPCSTR text);
			virtual void RenderText(Drawing::IFont *font, Drawing::Rectangle &rect, LPCSTR text);
			virtual void RenderText(Drawing::IFont *font, int x, int y, int w, int h, LPCSTR text);
			
			void RenderTextEx(Drawing::IFont *font, const Drawing::Point &point, LPCSTR text, ...);
			void RenderTextEx(Drawing::IFont *font, int x, int y, LPCSTR text, ...);
			void RenderTextEx(Drawing::IFont *font, Drawing::Rectangle &rect, LPCSTR text, ...);
			void RenderTextEx(Drawing::IFont *font, int x, int y, int w, int h, LPCSTR text, ...);

			virtual void Fill(const Drawing::Point &point);
			virtual void Fill(int x, int y);
			virtual void Fill(Drawing::Rectangle &rect);
			virtual void Fill(int x, int y, int w, int h);
			
		protected:
			void Flush();
			void AddVertex(int x, int y);
			void AddVertex(int x, int y, float u, float v);
		
			IDirect3DDevice9 *device;
			LPD3DXSPRITE sprite;

			ITexture *texture;
			
			static const int maxVertices = 1024;
			int verticesNum;
			Vertex2D vertices[maxVertices];
		};
	}
}

#endif