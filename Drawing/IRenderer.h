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
			
			void SetRenderColor(Color color) { this->color = color; }
			Color GetRenderColor() { return color; }
			void SetRenderRectangle(Rectangle &rect) { renderRect = rect; }
			Rectangle GetRenderRectangle() { return renderRect; }
			
			virtual void RenderTexture(ITexture *texture, const Point &point) = 0;
			virtual void RenderTexture(ITexture *texture, int x, int y) = 0;

			virtual Size MeasureText(IFont *font, const String &text) = 0;

			virtual void RenderText(IFont *font, const Point &point, const String &text) = 0;
			virtual void RenderText(IFont *font, int x, int y, const String &text) = 0;
			virtual void RenderText(IFont *font, Rectangle &rect, const String &text) = 0;
			virtual void RenderText(IFont *font, int x, int y, int w, int h, const String &text) = 0;
					
			virtual void Fill(const Point &point) = 0;
			virtual void Fill(int x, int y) = 0;
			virtual void Fill(Rectangle &rect) = 0;
			virtual void Fill(int x, int y, int w, int h) = 0;
			
			virtual void FillGradient(const Point &point, Color &to) = 0;
			virtual void FillGradient(int x, int y, Color &to) = 0;
			virtual void FillGradient(Rectangle &rect, Color &to) = 0;
			virtual void FillGradient(int x, int y, int w, int h, Color &to) = 0;

		protected:
			Color color;
			Rectangle renderRect;
		};
	}
}

#endif