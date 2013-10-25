#ifndef OSHGUI_DRAWING_FONTGLYPH_HPP
#define OSHGUI_DRAWING_FONTGLYPH_HPP

#include "Image.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/*!
		\brief
			internal class representing a single font glyph.

			For TrueType fonts initially all FontGlyph's are empty
			(getImage() will return 0), but they are filled by demand.
		*/
		class OSHGUI_EXPORT FontGlyph
		{
		public:
			//! Constructor.
			FontGlyph(float _advance = 0.0f, ImagePtr _image = nullptr, bool _valid = false)
				: image(std::move(_image)),
				  advance(_advance),
				  valid(_valid)
			{

			}

			//! Return the CEGUI::Image object rendered for this glyph.
			const ImagePtr& GetImage() const
			{
				return image;
			}

			//! Return the scaled pixel size of the glyph.
			SizeF GetSize(float scaleX, float scaleY) const
			{
				return SizeF(GetWidth(scaleX), GetHeight(scaleY));
			}

			//! Return the scaled width of the glyph.
			float GetWidth(float scaleX) const
			{
				return image->GetSize().Width * scaleX;
			}

			//! Return the scaled height of the glyph.
			float GetHeight(float scaleY) const
			{
				return image->GetSize().Height * scaleY;
			}

			/*!
			\brief
				Return the rendered advance value for this glyph.

				The rendered advance value is the total number of pixels from the
				current pen position that will be occupied by this glyph when rendered.
			*/
			float GetRenderedAdvance(float scaleX) const
			{
				return (image->GetSize().Width + image->GetOffset().X) * scaleX;
			}

			/*!
			\brief
				Return the horizontal advance value for the glyph.

				The returned value is the number of pixels the pen should move
				horizontally to position itself ready to render the next glyph.  This
				is not always the same as the glyph image width or rendererd advance,
				since it allows for horizontal overhangs.
			*/
			float GetAdvance(float scaleX = 1.0f) const
			{
				return advance * scaleX;
			}

			//! Set the horizontal advance value for the glyph.
			void SetAdvance(float advance)
			{
				this->advance = advance;
			}

			//! Set the CEGUI::Image object rendered for this glyph.
			void SetImage(const ImagePtr &image)
			{
				this->image = image;
			}

			//! mark the FontGlyph as valid
			void SetValid(bool valid)
			{
				this->valid = valid;
			}

			//! return whether the FontGlyph is marked as valid
			bool IsValid() const
			{
				return valid;
			}

		private:
			//! The image which will be rendered for this glyph.
			ImagePtr image;
			//! Amount to advance the pen after rendering this glyph
			float advance;
			//! says whether this glyph info is actually valid
			bool valid;
		};
	}
}

#endif
