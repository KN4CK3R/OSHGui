#ifndef OSHGUI_DRAWING_BASICIMAGE_HPP
#define OSHGUI_DRAWING_BASICIMAGE_HPP

#include "Image.hpp"
#include "Rectangle.hpp"
#include "Texture.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class OSHGUI_EXPORT BasicImage : public Image
		{
		public:
			BasicImage();
			BasicImage(TexturePtr &texture, const RectangleF &area, const PointF &pixelOffset, const AutoScaleMode autoscaled, const SizeF &nativeResolution);

			void SetTexture(TexturePtr &texture);
			void SetArea(const RectangleF &area);
			void SetOffset(const PointF &offset);
			void setAutoScaled(const AutoScaleMode autoscaled);
			void SetNativeResolution(const SizeF &nativeResolution);

			virtual const SizeF& GetRenderedSize() const override;
			virtual const PointF& GetRenderedOffset() const override;
			virtual void Render(GeometryBuffer &buffer, const RectangleF &area, const RectangleF *clip, const ColorRectangle &colors) const override;

			virtual void notifyDisplaySizeChanged(const SizeF &size) override;

		protected:
			//! Texture used by this image.
			mutable TexturePtr texture;
			//! Actual pixel size.
			SizeF pixelSize;
			//! Rect defining texture co-ords for this image.
			RectangleF area;
			//! Defined pixel offset
			PointF pixelOffset;
			//! Whether image is auto-scaled or not and how.
			AutoScaleMode autoScaleMode;
			//! Native resolution used for autoscaling.
			SizeF nativeResolution;
			//! SizeF after having autoscaling applied.
			SizeF scaledSize;
			//! Offset after having autoscaling applied.
			PointF scaledOffset;
		};
	}
}

#endif
