/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_COLOR_HPP
#define OSHGUI_DRAWING_COLOR_HPP

#include "../Exports.hpp"
#include <cstdint>

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Stellt eine ARGB-Farbe (Alpha, Rot, Grün, Blau) dar.
		 */
		class OSHGUI_EXPORT Color
		{
		public:
			union
			{
				struct
				{
					uint8_t B;
					uint8_t G;
					uint8_t R;
					uint8_t A;
				};
				uint32_t ARGB;
			};

			/**
			 * Legt eine leere Farbe an (ARGB = 0)
			 */
			Color();
			/**
			 * Legt eine Farbe mit dem angegeben ARGB Wert an.
			 *
			 * @param argb
			 */
			Color(uint32_t argb);
			/**
			 * Legt eine Farbe mit den Werten für Rot, Grün und Blau an.
			 *
			 * @param red
			 * @param green
			 * @param blue
			 */
			Color(uint8_t red, uint8_t green, uint8_t blue);
			/**
			 * Legt eine Farbe mit den Werten für Alpha, Rot, Grün und Blau an.
			 *
			 * @param alpha
			 * @param red
			 * @param green
			 * @param blue
			 */
			Color(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);

			Color& operator+=(const Color &rhs);
			Color& operator-=(const Color &rhs);
			Color& operator*=(const Color &rhs);
			Color& operator*=(float rhs);

			/**
			 * Gibt eine leere Farbe (ARGB = 0) zurück.
		     */
			static Color Empty();

			/**
			 * Gibt die Farbe Rot zurück (255, 255, 0, 0)
			 */
			static Color Red();
			/**
			 * Gibt die Farbe Lime zurück (255, 0, 255, 0)
			 */
			static Color Lime();
			/**
			 * Gibt die Farbe Blau zurück (255, 0, 0, 255)
			 */
			static Color Blue();

			/**
			 * Gibt die Farbe Schwarz zurück (255, 0, 0, 0)
			 */
			static Color Black();
			/**
			 * Gibt die Farbe Grau zurück (255, 128, 128, 128)
			 */
			static Color Grey();
			/**
			 * Gibt die Farbe Weiß zurück (255, 255, 255, 255)
			 */
			static Color White();

			/**
			 * Gibt die Farbe Gelb zurück (255, 255, 255, 0)
			 */
			static Color Yellow();
			/**
			 * Gibt die Farbe Fuchsia zurück (255, 255, 0, 255)
			 */
			static Color Fuchsia();
			/**
			 * Gibt die Farbe Cyan zurück (255, 0, 255, 255)
			 */
			static Color Cyan();
			/**
			 * Gibt die Farbe Orange zurück (255, 255, 125, 0)
			 */
			static Color Orange();

			/**
			 * Gibt die Farbe Maroon zurück (255, 128, 0, 0)
			 */
			static Color Maroon();
			/**
			 * Gibt die Farbe Grün zurück (255, 0, 128, 0)
			 */
			static Color Green();
			/**
			 * Gibt die Farbe Navy zurück (255, 0, 0, 128)
			 */
			static Color Navy();

			/**
			 * Berechnet den Farbton der Farbe.
			 *
			 * @return der Farbton
			 */
			float Hue() const;
			/**
			 * Berechnet die Sättigung der Farbe.
			 *
			 * @return die Sättigung
			 */
			float Saturation() const;
			/**
			 * Berechnet die Helligkeit der Farbe.
			 *
			 * @return die Helligkeit
			 */
			float Brightness() const;

			/**
			 * Wandelt HSB Farbinformationen in ein Color-Objekt um.
			 *
			 * @param hue der Farbton
			 * @param saturation die Sättigugn
			 * @param brightness die Helligkeit
			 * @return color
			 */
			static Color FromHSB(float hue, float saturation, float brightness);
		};

		bool operator==(const Color &lhs, const Color &rhs);
		bool operator!=(const Color &lhs, const Color &rhs);
		const Color operator+(const Color &lhs, const Color &rhs);
		const Color operator-(const Color &lhs, const Color &rhs);
		const Color operator*(const Color &lhs, const Color &rhs);
		const Color operator*(const Color &lhs, float rhs);
	}
}

#endif