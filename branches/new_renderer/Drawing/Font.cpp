#include "Font.hpp"
#include "../Application.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		// amount of bits in a uint
		const auto BITS_PER_UINT = sizeof(std::uint32_t) * 8;
		// must be a power of two
		const auto GLYPHS_PER_PAGE = 256;
		//---------------------------------------------------------------------------
		Font::Font()
			: ascender(0.0f),
			  descender(0.0f),
			  height(0.0f),
			  maximumCodepoint(0),
			  scalingHorizontal(1),
			  scalingVertical(1)
		{
			
		}
		//---------------------------------------------------------------------------
		Font::~Font()
		{
			
		}
		//---------------------------------------------------------------------------
		void Font::SetMaxCodepoint(std::uint32_t codepoint)
		{
			loadedGlyphPages.clear();

			maximumCodepoint = codepoint;

			auto pages = (codepoint + GLYPHS_PER_PAGE) / GLYPHS_PER_PAGE;
			auto size = (pages + BITS_PER_UINT - 1) / BITS_PER_UINT;

			loadedGlyphPages.resize(size * sizeof(std::uint32_t));
		}
		//---------------------------------------------------------------------------
		const FontGlyph* Font::GetGlyphData(std::uint32_t codepoint) const
		{
			if (codepoint > maximumCodepoint)
			{
				return nullptr;
			}

			auto glyph = FindFontGlyph(codepoint);

			if (!loadedGlyphPages.empty())
			{
				auto page = codepoint / GLYPHS_PER_PAGE;
				auto mask = 1 << (page & (BITS_PER_UINT - 1));
				if (!(loadedGlyphPages[page / BITS_PER_UINT] & mask))
				{
					loadedGlyphPages[page / BITS_PER_UINT] |= mask;
					Rasterise(codepoint & ~(GLYPHS_PER_PAGE - 1), codepoint | (GLYPHS_PER_PAGE - 1));
				}
			}

			return glyph;
		}
		//---------------------------------------------------------------------------
		const FontGlyph* Font::FindFontGlyph(const std::uint32_t codepoint) const
		{
			auto pos = glyphMap.find(codepoint);
			return (pos != glyphMap.end()) ? &pos->second : nullptr;
		}
		//---------------------------------------------------------------------------
		float Font::GetTextExtent(const Misc::AnsiString &text, float scaleX) const
		{
			float current = 0.f;
			float advance = 0.f;

			for (auto c : text)
			{
				auto glyph = GetGlyphData(c);
				if (glyph)
				{
					auto width = glyph->GetRenderedAdvance(scaleX);

					if (advance + width > current)
					{
						current = advance + width;
					}

					advance += glyph->GetAdvance(scaleX);
				}
			}

			return std::max(advance, current);
		}
		//---------------------------------------------------------------------------
		float Font::GetTextAdvance(const Misc::AnsiString &text, float scaleX) const
		{
			float advance = 0.0f;

			for (auto c : text)
			{
				if (auto glyph = GetGlyphData(c))
				{
					advance += glyph->GetAdvance(scaleX);
				}
			}

			return advance;
		}
		//---------------------------------------------------------------------------
		size_t Font::GetCharAtPixel(const Misc::AnsiString &text, size_t start, float pixel, float scaleX) const
		{
			float current = 0.f;
			auto length = text.length();

			if (pixel <= 0.f || length <= start)
			{
				return start;
			}

			for (auto c = start; c < length; ++c)
			{
				auto glyph = GetGlyphData(text[c]);
				if (glyph)
				{
					current += glyph->GetAdvance(scaleX);

					if (pixel < current)
					{
						return c;
					}
				}
			}

			return length;
		}
		//---------------------------------------------------------------------------
		float Font::DrawText(GeometryBuffer &buffer, const Misc::AnsiString &text, const PointF &position, const RectangleF *clip, const ColorRectangle &colors, const float spaceExtra, const float scaleX, const float scaleY) const
		{
			auto base = position.Y + GetBaseline(scaleY);
			auto glyphPosition(position);

			for (auto c : text)
			{
				const FontGlyph* glyph;
				if ((glyph = GetGlyphData(c)))
				{
					auto image = glyph->GetImage();
					glyphPosition.Y = base - (image->GetOffset().Y - image->GetOffset().Y * scaleY);
					image->Render(buffer, RectangleF(glyphPosition, glyph->GetSize(scaleX, scaleY)), clip, colors);
					glyphPosition.X += glyph->GetAdvance(scaleX);// - 1.f;

					if (c == ' ')
					{
						glyphPosition.X += spaceExtra;
					}
				}
			}

			return glyphPosition.X;
		}
		//---------------------------------------------------------------------------
		void Font::Rasterise(std::uint32_t startCodepoint, std::uint32_t endCodepoint) const
		{
			
		}
		//---------------------------------------------------------------------------
	}
}
