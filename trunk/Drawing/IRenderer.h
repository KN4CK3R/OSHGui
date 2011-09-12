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

			virtual std::shared_ptr<ITexture> CreateNewTexture() = 0;
			virtual std::shared_ptr<IFont> CreateNewFont() = 0;
			std::shared_ptr<IFont> GetDefaultFont() { return defaultFont; }
			
			virtual void SetRenderColor(Color color) { this->color = color; }
			virtual Color& GetRenderColor() { return color; }
			virtual void SetRenderRectangle(Rectangle &rect) { renderRect = rect; }
			virtual Rectangle& GetRenderRectangle() { return renderRect; }
			
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, const Point &point) = 0;
			virtual void RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y) = 0;

			virtual Size MeasureText(const std::shared_ptr<IFont> &font, const Misc::UnicodeString &text) = 0;

			virtual void RenderText(const std::shared_ptr<IFont> &font, const Point &point, const Misc::UnicodeString &text) = 0;
			virtual void RenderText(const std::shared_ptr<IFont> &font, int x, int y, const Misc::UnicodeString &text) = 0;
			virtual void RenderText(const std::shared_ptr<IFont> &font, Rectangle &rect, const Misc::UnicodeString &text) = 0;
			virtual void RenderText(const std::shared_ptr<IFont> &font, int x, int y, int w, int h, const Misc::UnicodeString &text) = 0;
					
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
			std::shared_ptr<IFont> defaultFont;
		};
	}
}

#endif