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
			
			void RenderTexture(const Drawing::Point &point, Drawing::ITexture *texture);
			virtual void RenderTexture(int x, int y, Drawing::ITexture *texture);

			virtual Size MeasureText(IFont *font, const String &text);

			virtual void RenderText(IFont *font, const Point &point, const String &text);
			virtual void RenderText(IFont *font, int x, int y, const String &text);
			virtual void RenderText(IFont *font, Rectangle &rect, const String &text);
			virtual void RenderText(IFont *font, int x, int y, int w, int h, const String &text);

			virtual void Fill(const Point &point);
			virtual void Fill(int x, int y);
			virtual void Fill(Rectangle &rect);
			virtual void Fill(int x, int y, int w, int h);

			virtual void FillGradient(const Point &point, Color &to);
			virtual void FillGradient(int x, int y, Color &to);
			virtual void FillGradient(Rectangle &rect, Color &to);
			virtual void FillGradient(int x, int y, int w, int h, Color &to);
			
		private:
			void Flush();
		
			ID3D11Device *device;
		};
	}
}

#endif