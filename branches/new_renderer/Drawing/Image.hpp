#ifndef OSHGUI_DRAWING_IMAGE2_HPP
#define OSHGUI_DRAWING_IMAGE2_HPP

#include "../Misc/Strings.hpp"
#include "../Misc/RawDataContainer.hpp"
#include "Texture.hpp"
#include "GeometryBuffer.hpp"
#include "ColorRectangle.hpp"
#include "Rectangle.hpp"
#include <memory>

namespace OSHGui
{
	namespace Drawing
	{
		class Graphics;
		class Font;

		class OSHGUI_EXPORT Image
		{
		public:
			Image();
			Image(TexturePtr texture);
			Image(TexturePtr texture, RectangleF area, PointF offset);
			virtual ~Image();

			static std::shared_ptr<Image> FromFile(const Misc::AnsiString &filename);
			static std::shared_ptr<Image> FromMemory(const Misc::RawDataContainer &data);
			static std::shared_ptr<Image> FromBuffer(const void *data, const SizeF &size, Texture::PixelFormat format);

			virtual const SizeF& GetSize() const;
			virtual const PointF& GetOffset() const;

		protected:
			friend class Graphics;
			friend class Font;

			virtual void Render(GeometryBuffer &buffer, const RectangleF &area, const RectangleF *clip, const ColorRectangle &color);

		private:
			TexturePtr texture;
			RectangleF area;
			PointF offset;
			
			SizeF scaledSize;
			PointF scaledOffset;
		};

		typedef std::shared_ptr<Image> ImagePtr;
	}
}

#endif
