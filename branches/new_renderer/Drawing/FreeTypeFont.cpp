#include "FreeTypeFont.hpp"
#include "Texture.hpp"
#include "Application.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		// Pixels to put between glyphs
		const unsigned int GLYPH_PADDING = 2;
		// A multiplication coefficient to convert FT_Pos values into normal floats
		const float FT_POS_COEFFICIENT = 1.0f/64.0f;
		//---------------------------------------------------------------------------
		FT_Library freeType;
		int freeTypeUsageCounter = 0;
		//---------------------------------------------------------------------------
		FreeTypeFont::FreeTypeFont(const float _pointSize, const bool _antiAliased, const Misc::AnsiString &filename, const AutoScaleMode autoScaleMode, const SizeF &nativeResoultion, const float _lineSpacing)
			: Font(filename, autoScaleMode, nativeResoultion),
			  lineSpacing(_lineSpacing),
			  pointSize(_pointSize),
			  antiAliased(_antiAliased),
			  fontFace(nullptr)
		{
			if (!freeTypeUsageCounter++)
			{
				FT_Init_FreeType(&freeType);
			}

			UpdateFont();
		}
		//---------------------------------------------------------------------------
		FreeTypeFont::~FreeTypeFont()
		{
			Free();

			if (!--freeTypeUsageCounter)
			{
				FT_Done_FreeType(freeType);
			}
		}
		//---------------------------------------------------------------------------
		std::uint32_t FreeTypeFont::GetTextureSize(CodepointIterator start, CodepointIterator end) const
		{
			auto size = 32;
			auto maximum = Application::Instance()->GetRenderer_()->GetMaximumTextureSize();
			auto count = 0;

			while (size < maximum)
			{
				auto x = GLYPH_PADDING;
				auto y = GLYPH_PADDING;
				auto yb = GLYPH_PADDING;
				for (auto c = start; c != end; ++c)
				{
					if (c->second.GetImage())
					{
						continue;
					}

					if (FT_Load_Char(fontFace, c->first, FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT) != 0)
					{
						continue;
					}

					auto glyphWidth = (int)std::ceil(fontFace->glyph->metrics.width * FT_POS_COEFFICIENT) + GLYPH_PADDING;
					auto glyphHeight = (int)std::ceil(fontFace->glyph->metrics.height * FT_POS_COEFFICIENT) + GLYPH_PADDING;

					x += glyphWidth;
					if (x > size)
					{
						x = GLYPH_PADDING;
						y = yb;
					}
					auto yy = y + glyphHeight;
					if (yy > size)
					{
						goto too_small;
					}

					if (yy > yb)
					{
						yb = yy;
					}

					++count;
				}
				break;

			too_small:
				size *= 2;
			}

			return count ? size : 0;
		}
		//---------------------------------------------------------------------------
		void FreeTypeFont::Rasterise(std::uint32_t startCodepoint, std::uint32_t endCodepoint) const
		{
			auto start = glyphMap.lower_bound(startCodepoint);
			if (start == glyphMap.end())
			{
				return;
			}

			auto bck = start;
			auto end = glyphMap.upper_bound(endCodepoint);
			while (true)
			{
				auto textureSize = GetTextureSize(start, end);
				if (textureSize == 0)
				{
					break;
				}

				auto texture = Application::Instance()->GetRenderer_()->CreateTexture(SizeF(textureSize, textureSize));
				glyphTextures.push_back(texture);

				std::vector<std::uint32_t> buffer(textureSize * textureSize);
				
				auto x = GLYPH_PADDING;
				auto y = GLYPH_PADDING;
				auto yb = GLYPH_PADDING;

				// Set to true when we finish rendering all glyphs we were asked to
				bool finished = false;
				// Set to false when we reach glyphMap.end() and we start going backward
				bool forward = true;

				/* To conserve texture space we will render more glyphs than asked,
				 * but never less than asked. First we render all glyphs from s to e
				 * and after that we render glyphs until we reach glyphMap.end(),
				 * and if there's still free texture space we will go backward
				 * from s until we hit glyphMap.begin().
				 */
				while (start != glyphMap.end())
				{
					// Check if we finished rendering all the required glyphs
					finished |= (start == end);

					// Check if glyph already rendered
					if (!start->second.GetImage())
					{
						if (FT_Load_Char(fontFace, start->first, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | (antiAliased ? FT_LOAD_TARGET_NORMAL : FT_LOAD_TARGET_MONO)) != 0)
						{
							RectangleF area(0, 0, 0, 0);
							PointF offset(0, 0);
							auto image = std::make_shared<BasicImage>(texture, area, offset, AutoScaleMode::Disabled, nativeResolution);
							glyphImages.push_back(image);
							start->second.SetImage(image);
						}
						else
						{
							auto glyphWidth = fontFace->glyph->bitmap.width + GLYPH_PADDING;
							auto glyphHeight = fontFace->glyph->bitmap.rows + GLYPH_PADDING;

							auto next = x + glyphWidth;
							if (next > textureSize)
							{
								x = GLYPH_PADDING;
								next = x + glyphWidth;
								y = yb;
							}

							auto bottom = y + glyphHeight;
							if (bottom > textureSize)
							{
								break;
							}

							DrawGlyphToBuffer(buffer.data() + (y * textureSize) + x, textureSize);

							RectangleF area(x, y, glyphWidth - GLYPH_PADDING, glyphHeight - GLYPH_PADDING);
							PointF offset(fontFace->glyph->metrics.horiBearingX * FT_POS_COEFFICIENT, -fontFace->glyph->metrics.horiBearingY * FT_POS_COEFFICIENT);

							auto image = std::make_shared<BasicImage>(texture, area, offset, AutoScaleMode::Disabled, nativeResolution);
							glyphImages.push_back(image);
							start->second.SetImage(image);

							x = next;
							if (bottom > yb)
							{
								yb = bottom;
							}
						}
					}

					if (forward)
					{
						if (++start == glyphMap.end())
						{
							finished = true;
							forward = false;
							start = bck;
						}
					}
					if (!forward)
					{
						if ((start == glyphMap.begin()) || (--start == glyphMap.begin()))
						{
							break;
						}
					}
				}

				texture->LoadFromMemory(buffer.data(), SizeF(textureSize, textureSize), Texture::PixelFormat::RGBA);

				if (finished)
				{
					break;
				}
			}
		}
		//---------------------------------------------------------------------------
		void FreeTypeFont::DrawGlyphToBuffer(std::uint32_t *buffer, std::uint32_t width) const
		{
			auto bitmap = &fontFace->glyph->bitmap;

			for (auto i = 0; i < bitmap->rows; ++i)
			{
				auto src = bitmap->buffer + (i * bitmap->pitch);
				switch (bitmap->pixel_mode)
				{
					case FT_PIXEL_MODE_GRAY:
						{
							auto dst = reinterpret_cast<std::uint8_t*>(buffer);
							for (int j = 0; j < bitmap->width; ++j)
							{
								*dst++ = 0xFF;
								*dst++ = 0xFF;
								*dst++ = 0xFF;
								*dst++ = *src++;
							}
						}
						break;
					case FT_PIXEL_MODE_MONO:
						for (auto j = 0; j < bitmap->width; ++j)
						{
							buffer[j] = (src[j / 8] & (0x80 >> (j & 7))) ? 0xFFFFFFFF : 0x00000000;
						}
						break;
					default:
						throw;
				}

				buffer += width;
			}
		}
		//---------------------------------------------------------------------------
		void FreeTypeFont::Free()
		{
			if (!fontFace)
			{
				return;
			}

			glyphMap.clear();
			glyphImages.clear();
			glyphTextures.clear();

			FT_Done_Face(fontFace);
			fontFace = nullptr;
		}
		//---------------------------------------------------------------------------
		void FreeTypeFont::UpdateFont()
		{
			Free();

			data.LoadFromFile(filename);

			FT_Error error;
			if ((error = FT_New_Memory_Face(freeType, data.GetDataPointer(), static_cast<FT_Long>(data.GetSize()), 0, &fontFace)) != 0)
			{
				/*CEGUI_THROW(GenericException("Failed to create face from font file '" +
					d_filename + "' error was: " +
					((error < FT_Err_Max) ? ft_errors[error] : "unknown error")));*/
				throw;
			}

			if (!fontFace->charmap)
			{
				FT_Done_Face(fontFace);
				fontFace = nullptr;
				/*CEGUI_THROW(GenericException(
					"The font '" + d_name + "' does not have a Unicode charmap, and "
					"cannot be used."));*/
				throw;
			}

			auto dpiHorizontal = static_cast<std::uint32_t>(Application::Instance()->GetRenderer_()->GetDisplayDPI().X);
			auto dpiVertical = static_cast<std::uint32_t>(Application::Instance()->GetRenderer_()->GetDisplayDPI().Y);

			auto hps = pointSize * 64.0f;
			auto vps = pointSize * 64.0f;
			if (autoScaleMode != AutoScaleMode::Disabled)
			{
				hps *= scalingHorizontal;
				vps *= scalingVertical;
			}

			if (FT_Set_Char_Size(fontFace, FT_F26Dot6(hps), FT_F26Dot6(vps), dpiHorizontal, dpiVertical) != 0)
			{
				// For bitmap fonts we can render only at specific point sizes.
				// Try to find nearest point size and use it, if that is possible
				auto ptSize72 = (pointSize * 72.0f) / dpiVertical;
				auto bestDelta = 99999.0f;
				auto bestSize = 0.0f;
				for (int i = 0; i < fontFace->num_fixed_sizes; i++)
				{
					float size = fontFace->available_sizes[i].size * FT_POS_COEFFICIENT;
					float delta = std::abs(size - ptSize72);
					if (delta < bestDelta)
					{
						bestDelta = delta;
						bestSize = size;
					}
				}

				if (bestSize <= 0.0f || FT_Set_Char_Size(fontFace, 0, FT_F26Dot6(bestSize * 64), 0, 0) != 0)
				{
					char size [20];
					//snprintf(size, sizeof(size), "%g", d_ptSize);
					/*CEGUI_THROW(GenericException("The font '" + d_name + "' cannot be "
						"rasterised at a size of " + size + " points, and cannot be "
						"used."));*/
					throw;
				}
			}

			if (fontFace->face_flags & FT_FACE_FLAG_SCALABLE)
			{
				float scaleY = fontFace->size->metrics.y_scale * FT_POS_COEFFICIENT * (1.0f / 65536.0f);
				ascender = fontFace->ascender * scaleY;
				descender = fontFace->descender * scaleY;
				height = fontFace->height * scaleY;
			}
			else
			{
				ascender = fontFace->size->metrics.ascender * FT_POS_COEFFICIENT;
				descender = fontFace->size->metrics.descender * FT_POS_COEFFICIENT;
				height = fontFace->size->metrics.height * FT_POS_COEFFICIENT;
			}

			if (lineSpacing > 0.0f)
			{
				height = lineSpacing;
			}

			InitialiseGlyphMap();
		}
		//---------------------------------------------------------------------------
		void FreeTypeFont::InitialiseGlyphMap()
		{
			FT_UInt index;
			auto codepoint = FT_Get_First_Char(fontFace, &index);
			auto maximum = codepoint;

			while (index)
			{
				if (maximum < codepoint)
				{
					maximum = codepoint;
				}

				glyphMap[codepoint] = FontGlyph();

				codepoint = FT_Get_Next_Char(fontFace, codepoint, &index);
			}

			SetMaxCodepoint(maximum);
		}
		//---------------------------------------------------------------------------
		const FontGlyph* FreeTypeFont::FindFontGlyph(const std::uint32_t codepoint) const
		{
			auto it = glyphMap.find(codepoint);
			if (it == glyphMap.end())
			{
				return nullptr;
			}

			if (!it->second.IsValid())
			{
				InitialiseFontGlyph(it);
			}

			return &it->second;
		}
		//---------------------------------------------------------------------------
		void FreeTypeFont::InitialiseFontGlyph(CodepointIterator it) const
		{
			if (FT_Load_Char(fontFace, it->first, FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT) != 0)
			{
				return;
			}

			auto advance = fontFace->glyph->metrics.horiAdvance * FT_POS_COEFFICIENT;

			it->second.SetAdvance(advance);
			it->second.SetValid(true);
		}
		//---------------------------------------------------------------------------
		float FreeTypeFont::GetPointSize() const
		{
			return pointSize;
		}
		//---------------------------------------------------------------------------
		void FreeTypeFont::SetPointSize(const float pointSize)
		{
			if (this->pointSize == pointSize)
			{
				return;
			}

			this->pointSize = pointSize;
			UpdateFont();
		}
		//---------------------------------------------------------------------------
		bool FreeTypeFont::IsAntiAliased() const
		{
			return antiAliased;
		}
		//---------------------------------------------------------------------------
		void FreeTypeFont::SetAntiAliased(const bool antiAliasing)
		{
			if (this->antiAliased == antiAliasing)
			{
				return;
			}

			this->antiAliased = antiAliasing;
			UpdateFont();
		}
		//---------------------------------------------------------------------------
	}
}
