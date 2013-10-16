#ifndef OSHGUI_DRAWING_FONT_HPP
#define OSHGUI_DRAWING_FONT_HPP

#include "FontGlyph.hpp"
#include "Color.hpp"
#include "../Misc/Strings.hpp"
#include <map>
#include <vector>
#include <memory>

namespace OSHGui
{
	namespace Drawing
	{
		/*!
		\brief
			Class that encapsulates a typeface.

			A Font object is created for each unique typeface required.
			The Font class provides methods for loading typefaces from various sources,
			and then for outputting text via the Renderer object.

			This class is not specific to any font renderer, it just provides the
			basic interfaces needed to manage fonts.
		*/
		class OSHGUI_EXPORT Font
		{
		public:
			//! Destructor.
			virtual ~Font();

			/*!
			\brief
				Return whether this Font can draw the specified code-point

			\param cp
				std::uint32_t code point that is the subject of the query.

			\return
				true if the font contains a mapping for code point \a cp,
				false if it does not contain a mapping for \a cp.
			*/
			bool IsCodepointAvailable(std::uint32_t cp) const
			{
				return (glyphMap.find(cp) != glyphMap.end());
			}

			/*!
			\brief
				Draw text into a specified area of the display.

			\param buffer
				GeometryBuffer object where the geometry for the text be queued.

			\param text
				String object containing the text to be drawn.

			\param position
				Reference to a Vector2 object describing the location at which the text
				is to be drawn.

			\param clip_rect
				Rect object describing the clipping area for the drawing.
				No drawing will occur outside this Rect.

			\param colours
				ColourRect object describing the colours to be applied when drawing the
				text.  NB: The colours specified in here are applied to each glyph,
				rather than the text as a whole.

			\param space_extra
				Number of additional pixels of spacing to be added to space characters.

			\param x_scale
				Scaling factor to be applied to each glyph's x axis, where 1.0f is
				considered to be 'normal'.

			\param y_scale
				Scaling factor to be applied to each glyph's y axis, where 1.0f is
				considered to be 'normal'.

			\return
				The x co-ord where subsequent text should be rendered to ensure correct
				positioning (which is not possible to determine accurately by using the
				extent measurement functions).
			*/
			float DrawText(GeometryBuffer &buffer, const Misc::AnsiString &text, const PointF &position, const RectangleF *clip, const ColorRectangle &colors, const float spaceExtra = 0.0f, const float scaleX = 1.0f, const float scaleY = 1.0f) const;

			/*!
			\brief
				Set the native resolution for this Font

			\param size
				Size object describing the new native screen resolution for this Font.
			*/
			void SetNativeResolution(const SizeF &resolution);

			/*!
			\brief
				Return the native display size for this Font.  This is only relevant if
				the Font is being auto-scaled.

			\return
				Size object describing the native display size for this Font.
			*/
			const SizeF& GetNativeResolution() const;

			/*!
			\brief
				Enable or disable auto-scaling for this Font.

			\param auto_scaled
				AutoScaledMode describing how this font should be auto scaled

			\see AutoScaledMode
			*/
			void SetAutoScaled(const AutoScaleMode autoScaleMode);

			/*!
			\brief
				Checks whether this font is being auto-scaled and how.

			\return
				AutoScaledMode describing how this font should be auto scaled
			*/
			AutoScaleMode GetAutoScaled() const;

			/*!
			\brief
				Notify the Font that the display size may have changed.

			\param size
				Size object describing the display resolution
			*/
			virtual void NotifyDisplaySizeChanged(const SizeF &size);

			/*!
			\brief
				Return the pixel line spacing value for.

			\param y_scale
				Scaling factor to be applied to the line spacing, where 1.0f
				is considered to be 'normal'.

			\return
				Number of pixels between vertical base lines, i.e. The minimum
				pixel space between two lines of text.
			*/
			float GetLineSpacing(float scaleY = 1.0f) const
			{
				return height * scaleY;
			}

			/*!
			\brief
				return the exact pixel height of the font.

			\param y_scale
				Scaling factor to be applied to the height, where 1.0f
				is considered to be 'normal'.

			\return
				float value describing the pixel height of the font without
				any additional padding.
			*/
			float GetFontHeight(float scaleY = 1.0f) const
			{
				return (ascender - descender) * scaleY;
			}

			/*!
			\brief
				Return the number of pixels from the top of the highest glyph
				to the baseline

			\param y_scale
				Scaling factor to be applied to the baseline distance, where 1.0f
				is considered to be 'normal'.

			\return
				pixel spacing from top of front glyphs to baseline
			*/
			float GetBaseline(float scaleY = 1.0f) const
			{
				return ascender * scaleY;
			}

			/*!
			\brief
				Return the pixel width of the specified text if rendered with
				this Font.

			\param text
				String object containing the text to return the rendered pixel
				width for.

			\param x_scale
				Scaling factor to be applied to each glyph's x axis when
				measuring the extent, where 1.0f is considered to be 'normal'.

			\return
				Number of pixels that \a text will occupy when rendered with
				this Font.

			\note
				The difference between the advance and the extent of a text string is
				important for numerous reasons. Picture some scenario where a glyph
				has a swash which extends way beyond the subsequent glyph - the text
				extent of those two glyphs is to the end of the swash on the first glyph
				whereas the advance of those two glyphs is to the start of a theoretical
				third glyph - still beneath the swash of the first glyph.
				The difference can basically be summarised as follows:
				- the extent is the total rendered width of all glyphs in the string.
				- the advance is the width to the point where the next character would
				  have been drawn.

			\see getTextAdvance
			*/
			float GetTextExtent(const Misc::AnsiString &text, float scaleX = 1.0f) const;

			/*!
			\brief
				Return pixel advance of the specified text when rendered with this Font.

			\param text
				String object containing the text to return the pixel advance for.

			\param x_scale
				Scaling factor to be applied to each glyph's x axis when
				measuring the advance, where 1.0f is considered to be 'normal'.

			\return
				pixel advance of \a text when rendered with this Font.

			\note
				The difference between the advance and the extent of a text string is
				important for numerous reasons. Picture some scenario where a glyph
				has a swash which extends way beyond the subsequent glyph - the text
				extent of those two glyphs is to the end of the swash on the first glyph
				whereas the advance of those two glyphs is to the start of a theoretical
				third glyph - still beneath the swash of the first glyph.
				The difference can basically be summarised as follows:
				- the extent is the total rendered width of all glyphs in the string.
				- the advance is the width to the point where the next character would
				  have been drawn.

			\see getTextExtent
			*/
			float GetTextAdvance(const Misc::AnsiString &text, float scaleX = 1.0f) const;

			/*!
			\brief
				Return the index of the closest text character in String \a text
				that corresponds to pixel location \a pixel if the text were rendered.

			\param text
				String object containing the text.

			\param pixel
				Specifies the (horizontal) pixel offset to return the character
				index for.

			\param x_scale
				Scaling factor to be applied to each glyph's x axis when measuring
				the text extent, where 1.0f is considered to be 'normal'.

			\return
				Returns a character index into String \a text for the character that
				would be rendered closest to horizontal pixel offset \a pixel if the
				text were to be rendered via this Font.  Range of the return is from
				0 to text.length(), so may actually return an index past the end of
				the string, which indicates \a pixel was beyond the last character.
			*/
			size_t GetCharAtPixel(const Misc::AnsiString &text, float pixel, float scaleX = 1.0f) const
			{
				return GetCharAtPixel(text, 0, pixel, scaleX);
			}

			/*!
			\brief
				Return the index of the closest text character in String \a text,
				starting at character index \a start_char, that corresponds
				to pixel location \a pixel if the text were to be rendered.

			\param text
				String object containing the text.

			\param start_char
				index of the first character to consider.  This is the lowest
				value that will be returned from the call.

			\param pixel
				Specifies the (horizontal) pixel offset to return the character
				index for.

			\param x_scale
				Scaling factor to be applied to each glyph's x axis when measuring
				the text extent, where 1.0f is considered to be 'normal'.

			\return
				Returns a character index into String \a text for the character that
				would be rendered closest to horizontal pixel offset \a pixel if the
				text were to be rendered via this Font.  Range of the return is from
				0 to text.length(), so may actually return an index past the end of
				the string, which indicates \a pixel was beyond the last character.
			*/
			size_t GetCharAtPixel(const Misc::AnsiString& text, size_t start_char, float pixel, float x_scale = 1.0f) const;

			/*!
			\brief
				Return a pointer to the glyphDat struct for the given codepoint,
				or 0 if the codepoint does not have a glyph defined.

			\param codepoint
				std::uint32 codepoint to return the glyphDat structure for.

			\return
				Pointer to the glyphDat struct for \a codepoint, or 0 if no glyph
				is defined for \a codepoint.
			*/
			const FontGlyph* GetGlyphData(std::uint32_t codepoint) const;

		protected:
			//! Constructor.
			Font(const AutoScaleMode auto_scaled, const SizeF& native_res);

			/*!
			\brief
				This function prepares a certain range of glyphs to be ready for
				displaying. This means that after returning from this function
				glyphs from glyphMap[start_codepoint] to glyphMap[end_codepoint]
				should have their d_image member set. If there is an error
				during rasterisation of some glyph, it's okay to leave the
				d_image field set to NULL, in which case such glyphs will
				be skipped from display.
			\param start_codepoint
				The lowest codepoint that should be rasterised
			\param end_codepoint
				The highest codepoint that should be rasterised
			*/
			virtual void Rasterise(std::uint32_t startCodepoint, std::uint32_t endCodepoint) const;

			//! Update the font as needed, according to the current parameters.
			virtual void UpdateFont() = 0;

			/*!
			\brief
				Set the maximal glyph index. This reserves the respective
				number of bits in the d_glyphPageLoaded array.
			*/
			void SetMaxCodepoint(std::uint32_t codepoint);

			//! finds FontGlyph in map and returns it, or 0 if none.
			virtual const FontGlyph* FindFontGlyph(const std::uint32_t codepoint) const;

			//! Name of the file used to create this font (font file or imagset)
			Misc::AnsiString filename;

			//! maximal font ascender (pixels above the baseline)
			float ascender;
			//! maximal font descender (negative pixels below the baseline)
			float descender;
			//! (ascender - descender) + linegap
			float height;

			//! which mode should we use for auto-scaling
			AutoScaleMode autoScaleMode;
			//! native resolution for this Font.
			SizeF nativeResolution;
			//! current horizontal scaling factor.
			float scalingHorizontal;
			//! current vertical scaling factor.
			float scalingVertical;

			//! Maximal codepoint for font glyphs
			std::uint32_t maximumCodepoint;

			/*!
			\brief
				This bitmap holds information about loaded 'pages' of glyphs.
				A glyph page is a set of 256 codepoints, starting at 256-multiples.
				For example, the 1st glyph page is 0-255, fourth is 1024-1279 etc.
				When a specific glyph is required for painting, the corresponding
				bit is checked to see if the respective page has been rasterised.
				If not, the rasterise() method is invoked, which prepares the
				glyphs from the respective glyph page for being painted.

				This array is big enough to hold at least max_codepoint bits.
				If this member is NULL, all glyphs are considered pre-rasterised.
			*/
			mutable std::vector<std::uint32_t> loadedGlyphPages;

			//! Definition of CodepointMap type.
			typedef std::map<std::uint32_t, FontGlyph> CodepointMap;
			typedef CodepointMap::iterator CodepointIterator;
			//! Contains mappings from code points to Image objects
			mutable CodepointMap glyphMap;
		};

		typedef std::shared_ptr<Font> FontPtr;
	}
}

#endif
