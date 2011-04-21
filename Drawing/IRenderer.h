#ifndef __OSHGUI_DRAWING_IRENDERER_H__
#define __OSHGUI_DRAWING_IRENDERER_H__

#include "Color.h"
#include "Point.h"
#include "Size.h"
#include "Rectangle.h"
#include "ITexture.h"
#include "IFont.h"

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
			
			void SetRenderColor(Drawing::Color color) { this->color = color; }
			Drawing::Color GetRenderColor() { return color; }
			void SetRenderRectangle(Drawing::Rectangle &rect) { renderRect = rect; }
			Drawing::Rectangle GetRenderRectangle() { return renderRect; }
			
			virtual void RenderTexture(Drawing::ITexture *texture, const Drawing::Point &point) = 0;
			virtual void RenderTexture(Drawing::ITexture *texture, int x, int y) = 0;

			virtual Drawing::Size MeasureText(Drawing::IFont *font, const char *text) = 0;
			virtual Drawing::Size MeasureTextEx(Drawing::IFont *font, const char *text, ...) = 0;

			virtual void RenderText(Drawing::IFont *font, const Drawing::Point &point, const char *text) = 0;
			virtual void RenderText(Drawing::IFont *font, int x, int y, const char *text) = 0;
			virtual void RenderText(Drawing::IFont *font, Drawing::Rectangle &rect, const char *text) = 0;
			virtual void RenderText(Drawing::IFont *font, int x, int y, int w, int h, const char *text) = 0;
			
			virtual void Fill(const Drawing::Point &point) = 0;
			virtual void Fill(int x, int y) = 0;
			virtual void Fill(Drawing::Rectangle &rect) = 0;
			virtual void Fill(int x, int y, int w, int h) = 0;
			
		protected:
			Drawing::Color color;
			Drawing::Rectangle renderRect;
		};
	}
}

#endif