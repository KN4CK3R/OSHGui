#ifndef __OSHGUI_DRAWING_RENDERERDX10_H__
#define __OSHGUI_DRAWING_RENDERERDX10_H__

#include <d3d11.h>

#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\IRenderer.h"

namespace OSHGui
{
	namespace Drawing
	{
		class RendererDX10 : public IRenderer
		{
		public:
			RendererDX10(ID3D10Device *device);
			virtual ~RendererDX10();
			
			virtual void SetRenderColor(D3DCOLOR color);
			
			void DrawTexture(const Drawing::Point &point, Drawing::ITexture *texture);
			virtual void DrawTexture(int x, int y, Drawing::ITexture *texture);

			virtual Drawing::Size MeasureText(Drawing::IFont *font, LPCSTR text);
			void DrawText(Drawing::IFont *font, const Drawing::Point &point, LPCSTR text);
			void DrawText(Drawing::IFont *font, int x, int y, LPCSTR text);
			void DrawText(Drawing::IFont *font, const Drawing::Rectangle &rectangle, LPCSTR text);
			virtual void DrawText(Drawing::IFont *font, int x, int y, int w, int h, LPCSTR text);
			
			void Fill(const Drawing::Point &point, D3DCOLOR color);
			void Fill(int x, int y, D3DCOLOR color);
			void Fill(Drawing::Rectangle &rect, D3DCOLOR color);
			virtual void Fill(int x, int y, int w, int h, D3DCOLOR color);
			
		private:
			void Flush();
		
			ID3D10Device *device;
			
			D3DCOLOR color;
		};
	}
}

#endif