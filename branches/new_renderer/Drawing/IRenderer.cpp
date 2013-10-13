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
		void IRenderer::SetRenderRectangle(const RectangleF &rect)
		{
			renderRect = rect;
		}
		//---------------------------------------------------------------------------
		const RectangleF& IRenderer::GetRenderRectangle() const
		{
			return renderRect;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void IRenderer::RenderTexture(const std::shared_ptr<ITexture> &texture, const PointF &point)
		{
			Drawing::SizeF size = texture->GetSize();
			RenderTexture(texture, point.X, point.Y, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y)
		{
			Drawing::SizeF size = texture->GetSize();
			RenderTexture(texture, x, y, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderTexture(const std::shared_ptr<ITexture> &texture, const PointF &point, const SizeF &size)
		{
			RenderTexture(texture, point.Left, point.Top, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderTexture(const std::shared_ptr<ITexture> &texture, const RectangleF &rect)
		{
			RenderTexture(texture, rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
		}
		//---------------------------------------------------------------------------
		SizeF IRenderer::MeasureText(const std::shared_ptr<IFont> &font, const Misc::AnsiString &text)
		{
			if (font == nullptr)
			{
				return SizeF();
			}
			
			return font->MeasureText(text);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderText(const std::shared_ptr<IFont> &font, const PointF &point, const Misc::AnsiString &text)
		{
			RenderText(font, point.X, point.Y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderText(const std::shared_ptr<IFont> &font, int x, int y, const Misc::AnsiString &text)
		{
			RenderText(font, x, y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderText(const std::shared_ptr<IFont> &font, const RectangleF &rectangle, const Misc::AnsiString &text)
		{
			RenderText(font, rectangle.GetLeft(), rectangle.GetTop(), rectangle.GetWidth(), rectangle.GetHeight(), text);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderText(const std::shared_ptr<IFont> &font, const PointF &location, const SizeF &size, const Misc::AnsiString &text)
		{
			RenderText(font, location.Left, location.Top, size.Width, size.Height, text);
		}
		//---------------------------------------------------------------------------
		void IRenderer::Fill(const PointF &point)
		{
			Fill(point.X, point.Y, 1, 1);
		}
		//---------------------------------------------------------------------------
		void IRenderer::Fill(const PointF &point, const SizeF &size)
		{
			Fill(point.X, point.Y, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void IRenderer::Fill(int x, int y)
		{
			Fill(x, y, 1, 1);
		}
		//---------------------------------------------------------------------------
		void IRenderer::Fill(const RectangleF &rect)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
		}
		//---------------------------------------------------------------------------
		void IRenderer::FillGradient(const PointF &point, const SizeF &size, Color to)
		{
			FillGradient(point.Left, point.Top, size.Width, size.Height, to);
		}
		//---------------------------------------------------------------------------
		void IRenderer::FillGradient(const RectangleF &rect, Color to)
		{
			FillGradient(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), to);
		}
		//---------------------------------------------------------------------------
		void IRenderer::RenderLine(const PointF &from, const PointF &to)
		{
			RenderLine(from.X, from.Y, to.X, to.Y);
		}
		//---------------------------------------------------------------------------
	}
}