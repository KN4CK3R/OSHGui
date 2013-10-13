#ifndef OSHGUI_DRAWING_IMAGE_HPP
#define OSHGUI_DRAWING_IMAGE_HPP

#include "GeometryBuffer.hpp"
#include "ColorRectangle.hpp"
#include "Rectangle.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		enum class AutoScaleMode
		{
			//! No auto scaling takes place
			Disabled,
			/*!
			Objects are auto scaled depending on their original height and current
			resolution height. Horizontal scaling is computed to keep aspect ratio.
			*/
			Vertical,
			/*!
			Objects are auto scaled depending on their original width and current
			resolution width. Horizontal scaling is computed to keep aspect ratio.
			*/
			Horizontal,
			/*!
			Objects are auto scaled by the smaller scaling factor of the two.
			*/
			Min,
			/*!
			Objects are auto scaled by the larger scaling factor of the two.
			*/
			Max,
			/*!
			Objects are auto scaled depending on their original size and current
			resolution. Both dimensions are scaled accordingly.

			This was the only auto scaling behavior available in 0.7 and it might
			break aspect ratios!
			*/
			Both
		};

		/*!
		\brief
			Interface for Image.

			In CEGUI, an Image is some object that can render itself into a given
			GeometryBuffer object.  This may be something as simple as a basic textured
			quad, or something more complex.
		*/
		class OSHGUI_EXPORT Image
		{
		public:
			virtual ~Image();

			virtual const SizeF& GetRenderedSize() const = 0;
			virtual const PointF& GetRenderedOffset() const = 0;

			virtual void Render(GeometryBuffer &buffer, const RectangleF &dest_area, const RectangleF *clip, const ColorRectangle &color) const = 0;

			virtual void notifyDisplaySizeChanged(const SizeF &size) = 0;

			void Render(GeometryBuffer &buffer, const PointF &position, const RectangleF *clip = nullptr) const
			{
				Render(buffer, RectangleF(position, GetRenderedSize()), clip, Color::White());
			}

			void Render(GeometryBuffer &buffer, const PointF &position, const RectangleF *clip, const ColorRectangle &colors) const
			{
				Render(buffer, RectangleF(position, GetRenderedSize()), clip, colors);
			}

			void Render(GeometryBuffer &buffer, const PointF &position, const SizeF &size, const RectangleF *clip = nullptr) const
			{
				Render(buffer, RectangleF(position, size), clip, Color::White());
			}

			void Render(GeometryBuffer &buffer, const PointF &position, const SizeF &size, const RectangleF *clip, const ColorRectangle &colors) const
			{
				Render(buffer, RectangleF(position, size), clip, colors);
			}

			/*!
			\brief
				Helper able to compute scaling factors for auto scaling

			\note
				This is mostly for internal use, unless you know what you are doing,
				please don't touch this method!
			 */
			static void ComputeScalingFactors(AutoScaleMode mode, const SizeF &size, const SizeF &nativeSize, float &scaleX, float &scaleY);
		};
	}
}

#endif
