#ifndef __OSHGUI_DRAWING_RENDERERDX11_H__
#define __OSHGUI_DRAWING_RENDERERDX11_H__

#include <d3d11.h>

#include "IRenderer.h"

namespace OSHGui
{
	namespace Drawing
	{
		class RendererDX11 : public IRenderer
		{
		public:
			RendererDX11(ID3D11Device *device);
			virtual ~RendererDX11();
			
			void DrawTexture(const Drawing::Point &point, Drawing::ITexture *texture);
			virtual void DrawTexture(int x, int y, Drawing::ITexture *texture);

			virtual Drawing::Size MeasureText(Drawing::IFont *font, const char *text);
			void DrawText(Drawing::IFont *font, const Drawing::Point &point, const char *text);
			void DrawText(Drawing::IFont *font, int x, int y, const char *text);
			void DrawText(Drawing::IFont *font, const Drawing::Rectangle &rectangle, const char *text);
			virtual void DrawText(Drawing::IFont *font, int x, int y, int w, int h, const char *text);
			
			void Fill(const Drawing::Point &point, D3DCOLOR color);
			void Fill(int x, int y, D3DCOLOR color);
			void Fill(Drawing::Rectangle &rect, D3DCOLOR color);
			virtual void Fill(int x, int y, int w, int h, D3DCOLOR color);
			
		private:
			void Flush();
		
			ID3D11Device *device;
		};
	}
}

#endif