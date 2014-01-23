/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_FONT_HPP
#define OSHGUI_DRAWING_FONT_HPP

#include "FontGlyph.hpp"
#include "Color.hpp"
#include "../Misc/Strings.hpp"
#include "Image.hpp"
#include <map>
#include <vector>
#include <memory>

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Die Klasse repr�sentiert eine Schrift(art).
		 */
		class OSHGUI_EXPORT Font
		{
		public:
			/**
			 * Destruktor.
			 */
			virtual ~Font();

			/**
			 * Pr�ft, ob der entsprechende Codepoint gezeichnet werden kann.
			 *
			 * \param cp Codepoint
			 * \return true, falls der Codepoint gezeichnet werden kann
			 */
			bool IsCodepointAvailable(uint32_t cp) const
			{
				return (glyphMap.find(cp) != glyphMap.end());
			}

			/**
			 * Zeichnet den Text in den GeometryBuffer.
			 *
			 * \param buffer GeometryBuffer
			 * \param text Text
			 * \param position Position des Textes
			 * \param clip Clippingregion
			 * \param colors Farbe(n) der Glyphen
			 * \param spaceExtra zus�tzlicher Platz bei einem Leerzeichen
			 * \param scaleX Skalierungsfaktor (default = 1)
			 * \param scaleY Skalierungsfaktor (default = 1)
			 * \return End-X Koordinate des gezeichneten Textes
			 */
			virtual float DrawText(GeometryBuffer &buffer, const Misc::AnsiString &text, const PointF &position, const RectangleF *clip, const ColorRectangle &colors, const float spaceExtra = 0.0f, const float scaleX = 1.0f, const float scaleY = 1.0f) const;

			/**
			 * Ruft die H�he einer Textzeile ab.
			 *
			 * \param scaleY Skalierungsfaktor (default = 1)
			 * \return H�he einer Textzeile
			 */
			float GetLineSpacing(float scaleY = 1.0f) const
			{
				return height * scaleY;
			}

			/**
			 * Ruft die H�he der Schrift in Pixeln ab.
			 *
			 * \param scaleY Skalierungsfaktor (default = 1)
			 * \return H�he der Schrift
			 */
			float GetFontHeight(float scaleY = 1.0f) const
			{
				return (ascender - descender) * scaleY;
			}

			/**
			 * Ruft die H�he des gr��ten Glyphen der Schrift in Pixeln ab.
			 *
			 * \param scaleY Skalierungsfaktor (default = 1)
			 * \return H�he des gr��ten Glyphen der Schrift
			 */
			float GetBaseline(float scaleY = 1.0f) const
			{
				return ascender * scaleY;
			}

			/**
			 * Ruft die Breite des gezeichneten Textes in Pixeln ab.
			 * Dies ist die tats�chlich verwendete Breite, w�hrend \a GetTextAdvance die theoretische Breite des Textes berechnet.
			 *
			 * \param scaleY Skalierungsfaktor (default = 1)
			 * \return Breite des gezeichneten Textes
			 * \sa GetTextAdvance
			 */
			virtual float GetTextExtent(const Misc::AnsiString &text, float scaleX = 1.0f) const;

			/**
			 * Ruft die theoretisch Breite des gezeichneten Textes in Pixeln ab. "Theoretisch" bedeutet, dass schr�ge Glyphen breiter sein k�nnen, wie ihre eigentliche Breite.
			 *
			 * \param scaleY Skalierungsfaktor (default = 1)
			 * \return Breite des gezeichneten Textes
			 * \sa GetTextExtent
			 */
			virtual float GetTextAdvance(const Misc::AnsiString &text, float scaleX = 1.0f) const;

			/**
			 * Ruft den Index des Zeichens ab, das am n�chsten zu dem angegebenen Pixel liegt.
			 *
			 * \param text Text
			 * \param pixel Pixel, dessen Index gesucht wird
			 * \param scaleX Skalierungsfaktor (default = 1)
			 * \return Index des gesuchten Zeichens [0, text.length()]
			 */
			size_t GetCharAtPixel(const Misc::AnsiString &text, float pixel, float scaleX = 1.0f) const
			{
				return GetCharAtPixel(text, 0, pixel, scaleX);
			}

			/**
			 * Ruft den Index des Zeichens ab, das am n�chsten zu dem angegebenen Pixel liegt.
			 *
			 * \param text Text
			 * \param start Startindex im Text
			 * \param pixel Pixel, dessen Index gesucht wird
			 * \param scaleX Skalierungsfaktor (default = 1)
			 * \return Index des gesuchten Zeichens [0, text.length()]
			 */
			size_t GetCharAtPixel(const Misc::AnsiString& text, size_t start, float pixel, float scaleX = 1.0f) const;

			/**
			 * Ruft den zu \a codepoint geh�renden Glyph ab und rastert ihn eventuell.
			 *
			 * \param codepoint Codepoint
			 * \return nullptr, falls der Glyph nicht existiert
			 */
			const FontGlyph* GetGlyphData(uint32_t codepoint) const;

		protected:
			/**
			 * Konstruktor der Klasse.
			 */
			Font();

			/**
			 * Initialisiert die Glyphen [\a startCodepoint, \a endCodepoint ]
			 *
			 * \param startCodepoint niedrigster Codepoint zum Rastern
			 * \param endCodepoint h�chster Codepoint zum Rastern
			 */
			virtual void Rasterise(uint32_t startCodepoint, uint32_t endCodepoint) const;

			/**
			 * Aktuallisiert die Schrift, falls sich ihre Eigenschaften ge�ndert haben.
			 */
			virtual void UpdateFont() = 0;

			/**
			 * Legt den maximalen Codepoint fest.
			 */
			void SetMaxCodepoint(uint32_t codepoint);

			/**
			 * Ruft den zugeh�rigen Glyphen ab.
			 *
			 * \param codepoint Codepoint
			 * \return nullptr, falls Glyph nicht vorhanden
			 */
			virtual const FontGlyph* FindFontGlyph(const uint32_t codepoint) const;

			float ascender;
			float descender;
			float height;

			float scalingHorizontal;
			float scalingVertical;

			uint32_t maximumCodepoint;

			mutable std::vector<uint32_t> loadedGlyphPages;

			typedef std::map<uint32_t, FontGlyph> CodepointMap;
			typedef CodepointMap::iterator CodepointIterator;
			mutable CodepointMap glyphMap;
		};

		typedef std::shared_ptr<Font> FontPtr;
	}
}

#endif
