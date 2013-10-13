/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_RENDERERDX11_HPP
#define OSHGUI_DRAWING_RENDERERDX11_HPP

#error DX11 not implemented, exclude all DX11 files

#include <d3d11.h>

#include "IRenderer.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class RendererDX11 : public IRenderer
		{
		public:
			RendererDX11(ID3D11Device *device);
			virtual ~RendererDX11();
			
			void RenderTexture(const Drawing::PointF &point, Drawing::ITexture *texture);
			virtual void RenderTexture(int x, int y, Drawing::ITexture *texture);

			virtual SizeF MeasureText(IFont *font, const Misc::UnicodeString &text);

			virtual void RenderText(IFont *font, const PointF &point, const Misc::UnicodeString &text);
			virtual void RenderText(IFont *font, int x, int y, const Misc::UnicodeString &text);
			virtual void RenderText(IFont *font, RectangleF &rect, const Misc::UnicodeString &text);
			virtual void RenderText(IFont *font, int x, int y, int w, int h, const Misc::UnicodeString &text);

			virtual void Fill(const PointF &point);
			virtual void Fill(int x, int y);
			virtual void Fill(RectangleF &rect);
			virtual void Fill(int x, int y, int w, int h);

			virtual void FillGradient(const PointF &point, Color &to);
			virtual void FillGradient(int x, int y, Color &to);
			virtual void FillGradient(RectangleF &rect, Color &to);
			virtual void FillGradient(int x, int y, int w, int h, Color &to);
			
		private:
			void Flush();
		
			ID3D11Device *device;
		};
	}
}

#endif