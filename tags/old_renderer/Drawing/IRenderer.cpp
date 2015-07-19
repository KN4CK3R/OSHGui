/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "IRenderer.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		IRenderer::~IRenderer()
		{

		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		std::shared_ptr<IFont> IRenderer::GetDefaultFont() const
		{
			return defaultFont;
		}
		//---------------------------------------------------------------------------
		void IRenderer::SetRenderColor(Color color)
		{
			this->color = color;
		}
		//---------------------------------------------------------------------------
		Color IRenderer::GetRenderColor() const
		{
			return color;
		}
		//---------------------------------------------------------------------------
		void IRenderer::SetRenderRectangle(const Rectangle &rect)
		{
			renderRect = rect;
		}
		//---------------------------------------------------------------------------
		const Rectangle& IRenderer::GetRenderRectangle() const
		{
			return renderRect;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void IRenderer::RenderTexture(const std::shared_ptr<ITexture> &texture, const Point &point)
		{
			Drawing::Size size = texture->GetSize();
			RenderTexture(texture, point.X, point.Y, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y)
		{
			Drawing::Size size = texture->GetSize();
			RenderTexture(texture, x, y, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderTexture(const std::shared_ptr<ITexture> &texture, const Point &point, const Size &size)
		{
			RenderTexture(texture, point.Left, point.Top, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderTexture(const std::shared_ptr<ITexture> &texture, const Rectangle &rect)
		{
			RenderTexture(texture, rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
		}
		//---------------------------------------------------------------------------
		Size IRenderer::MeasureText(const std::shared_ptr<IFont> &font, const Misc::AnsiString &text)
		{
			if (font == nullptr)
			{
				return Size();
			}
			
			return font->MeasureText(text);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderText(const std::shared_ptr<IFont> &font, const Point &point, const Misc::AnsiString &text)
		{
			RenderText(font, point.X, point.Y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderText(const std::shared_ptr<IFont> &font, int x, int y, const Misc::AnsiString &text)
		{
			RenderText(font, x, y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderText(const std::shared_ptr<IFont> &font, const Rectangle &rectangle, const Misc::AnsiString &text)
		{
			RenderText(font, rectangle.GetLeft(), rectangle.GetTop(), rectangle.GetWidth(), rectangle.GetHeight(), text);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderText(const std::shared_ptr<IFont> &font, const Point &location, const Size &size, const Misc::AnsiString &text)
		{
			RenderText(font, location.Left, location.Top, size.Width, size.Height, text);
		}
		//---------------------------------------------------------------------------
		void IRenderer::Fill(const Point &point)
		{
			Fill(point.X, point.Y, 1, 1);
		}
		//---------------------------------------------------------------------------
		void IRenderer::Fill(const Point &point, const Size &size)
		{
			Fill(point.X, point.Y, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void IRenderer::Fill(int x, int y)
		{
			Fill(x, y, 1, 1);
		}
		//---------------------------------------------------------------------------
		void IRenderer::Fill(const Rectangle &rect)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
		}
		//---------------------------------------------------------------------------
		void IRenderer::FillGradient(const Point &point, const Size &size, Color to)
		{
			FillGradient(point.Left, point.Top, size.Width, size.Height, to);
		}
		//---------------------------------------------------------------------------
		void IRenderer::FillGradient(const Rectangle &rect, Color to)
		{
			FillGradient(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), to);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderLine(const Point &from, const Point &to)
		{
			RenderLine(from.X, from.Y, to.X, to.Y);
		}
		//---------------------------------------------------------------------------
	}
}