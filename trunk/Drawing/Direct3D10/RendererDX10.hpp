/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2011 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_RENDERERDX10_HPP_
#define OSHGUI_DRAWING_RENDERERDX10_HPP_

#include <d3d11.h>

#include "IRenderer.hpp"

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

			virtual Size MeasureText(IFont *font, const Misc::UnicodeString &text);

			virtual void RenderText(IFont *font, const Point &point, const Misc::UnicodeString &text);
			virtual void RenderText(IFont *font, int x, int y, const Misc::UnicodeString &text);
			virtual void RenderText(IFont *font, Rectangle &rect, const Misc::UnicodeString &text);
			virtual void RenderText(IFont *font, int x, int y, int w, int h, const Misc::UnicodeString &text);
			
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
		
			ID3D10Device *device;
		};
	}
}

#endif