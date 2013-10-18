#include "BasicImage.hpp"
#include "GeometryBuffer.hpp"
#include "Texture.hpp"
#include "Vertex.hpp"
#include "ColorRectangle.hpp"
#include "../Application.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		BasicImage::BasicImage()
			: pixelSize(0.0f, 0.0f),
			  area(0.0f, 0.0f, 0.0f, 0.0f),
			  pixelOffset(0.0f, 0.0f),
			  autoScaleMode(AutoScaleMode::Disabled),
			  nativeResolution(640.0f, 480.0f),
			  scaledSize(0.0f, 0.0f),
			  scaledOffset(0.0f, 0.0f)
		{

		}
		//---------------------------------------------------------------------------
		BasicImage::BasicImage(TexturePtr &_texture, const RectangleF &_area, const PointF &_pixelOffset, const AutoScaleMode _autoScaleMode, const SizeF &_nativeResolution)
			: texture(_texture),
			  pixelSize(_area.GetSize()),
			  area(_area),
			  pixelOffset(_pixelOffset),
			  autoScaleMode(_autoScaleMode),
			  nativeResolution(_nativeResolution)
		{
			notifyDisplaySizeChanged(Application::Instance()->GetRenderer_()->GetDisplaySize());
		}
		//---------------------------------------------------------------------------
		void BasicImage::SetTexture(TexturePtr &texture)
		{
			this->texture = texture;
		}
		//---------------------------------------------------------------------------
		void BasicImage::SetArea(const RectangleF &area)
		{
			this->area = area;
			pixelSize = area.GetSize();

			if (autoScaleMode != AutoScaleMode::Disabled)
			{
				notifyDisplaySizeChanged(Application::Instance()->GetRenderer_()->GetDisplaySize());
			}
		}
		//---------------------------------------------------------------------------
		void BasicImage::SetOffset(const PointF &offset)
		{
			pixelOffset = offset;

			if (autoScaleMode != AutoScaleMode::Disabled)
			{
				notifyDisplaySizeChanged(Application::Instance()->GetRenderer_()->GetDisplaySize());
			}
		}
		//---------------------------------------------------------------------------
		void BasicImage::setAutoScaled(const AutoScaleMode autoScaleMode)
		{
			this->autoScaleMode = autoScaleMode;

			if (autoScaleMode != AutoScaleMode::Disabled)
			{
				notifyDisplaySizeChanged(Application::Instance()->GetRenderer_()->GetDisplaySize());
			}
			else
			{
				scaledSize = pixelSize;
				scaledOffset = pixelOffset;
			}
		}
		//---------------------------------------------------------------------------
		void BasicImage::SetNativeResolution(const SizeF &resolution)
		{
			nativeResolution = resolution;

			if (autoScaleMode != AutoScaleMode::Disabled)
			{
				notifyDisplaySizeChanged(Application::Instance()->GetRenderer_()->GetDisplaySize());
			}
		}
		//---------------------------------------------------------------------------
		const SizeF& BasicImage::GetRenderedSize() const
		{
			return scaledSize;
		}
		//---------------------------------------------------------------------------
		const PointF& BasicImage::GetRenderedOffset() const
		{
			return scaledOffset;
		}
		//---------------------------------------------------------------------------
		void BasicImage::Render(GeometryBuffer &buffer, const RectangleF &_area, const RectangleF *clip, const ColorRectangle &colors) const
		{
			auto destination(_area);
			destination.Offset(scaledOffset);

			auto final_rect(clip ? destination.GetIntersection(*clip) : destination);

			if ((final_rect.GetWidth() == 0) || (final_rect.GetHeight() == 0))
			{
				return;
			}

			auto &scale = texture->GetTexelScaling();
			std::pair<float, float> pixelSize(area.GetWidth() / destination.GetWidth(), area.GetHeight() / destination.GetHeight());

			RectangleF textureRectangle((area.GetLocation() + ((final_rect.GetLocation() - destination.GetLocation()) * pixelSize)) * scale,
									    (area.GetSize() + ((final_rect.GetSize() - destination.GetSize()) * pixelSize)) * scale);

			auto alignToPixels = [](float x)
			{
				return (float)(int)(( x ) + (( x ) > 0.0f ? 0.5f : -0.5f));
			};

			final_rect.SetLeft(alignToPixels(final_rect.GetLeft()));
			final_rect.SetTop(alignToPixels(final_rect.GetTop()));
			final_rect.SetHeight(alignToPixels(final_rect.GetHeight()));
			final_rect.SetWidth(alignToPixels(final_rect.GetWidth()));

			Vertex vertices[] = {
				{ Vector(final_rect.GetLeft(), final_rect.GetTop(), 0.0f),     colors.TopLeft,     PointF(textureRectangle.GetLeft(), textureRectangle.GetTop()) },
				{ Vector(final_rect.GetLeft(), final_rect.GetBottom(), 0.0f),  colors.BottomLeft,  PointF(textureRectangle.GetLeft(), textureRectangle.GetBottom()) },
				{ Vector(final_rect.GetRight(), final_rect.GetBottom(), 0.0f), colors.BottomRight, PointF(textureRectangle.GetRight(), textureRectangle.GetBottom()) },
				{ Vector(final_rect.GetRight(), final_rect.GetTop(), 0.0f),    colors.TopRight,    PointF(textureRectangle.GetRight(), textureRectangle.GetTop()) },
				{ Vector(final_rect.GetLeft(), final_rect.GetTop(), 0.0f),     colors.TopLeft,     PointF(textureRectangle.GetLeft(), textureRectangle.GetTop()) },
				{ Vector(final_rect.GetRight(), final_rect.GetBottom(), 0.0f), colors.BottomRight, PointF(textureRectangle.GetRight(), textureRectangle.GetBottom()) },
			};

			buffer.SetActiveTexture(texture);
			buffer.AppendGeometry(vertices, 6);
		}
		//---------------------------------------------------------------------------
		void BasicImage::notifyDisplaySizeChanged(const SizeF &size)
		{
			float scaleX;
			float scaleY;

			ComputeScalingFactors(autoScaleMode, size, nativeResolution, scaleX, scaleY);

			scaledSize.Width = pixelSize.Width * scaleX;
			scaledSize.Height = pixelSize.Height * scaleY;
			scaledOffset.X = pixelOffset.X * scaleX;
			scaledOffset.Y = pixelOffset.Y * scaleY;
		}
		//---------------------------------------------------------------------------
	}
}
