#ifndef __OSHGUI_DRAWING_IRENDERER_H__
#define __OSHGUI_DRAWING_IRENDERER_H__

#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\ITexture.h"
#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\IFont.h"

namespace OSHGui
{
	namespace Drawing
	{
		class IRenderer
		{
		public:
			virtual void Begin() = 0;
			virtual void End() = 0;

			virtual ITexture* CreateNewTexture() = 0;
			virtual IFont* CreateNewFont() = 0;
			
			void SetRenderColor(D3DCOLOR color) { this->color = color; }
			void SetRenderOffset(Drawing::Point &point) { renderOffset = point; }
			Drawing::Point GetRenderOffset() { return renderOffset; }
			
			virtual void RenderTexture(Drawing::ITexture *texture, const Drawing::Point &point) = 0;
			virtual void RenderTexture(Drawing::ITexture *texture, int x, int y) = 0;

			virtual Drawing::Size MeasureText(Drawing::IFont *font, LPCWSTR text) = 0;
			virtual Drawing::Size MeasureTextEx(Drawing::IFont *font, LPCWSTR text, ...) = 0;

			virtual void RenderText(Drawing::IFont *font, const Drawing::Point &point, LPCWSTR text) = 0;
			virtual void RenderText(Drawing::IFont *font, int x, int y, LPCWSTR text) = 0;
			virtual void RenderText(Drawing::IFont *font, Drawing::Rectangle &rect, LPCWSTR text) = 0;
			virtual void RenderText(Drawing::IFont *font, int x, int y, int w, int h, LPCWSTR text) = 0;
			
			virtual void Fill(const Drawing::Point &point) = 0;
			virtual void Fill(int x, int y) = 0;
			virtual void Fill(Drawing::Rectangle &rect) = 0;
			virtual void Fill(int x, int y, int w, int h) = 0;
			
		protected:
			D3DCOLOR color;
			Drawing::Point renderOffset;
		};
	}
}

#endif