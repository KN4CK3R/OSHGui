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
		 * Stellt eine ARGB-Farbe (Alpha, Rot, Gr�n, Blau) dar.
		 */
		class OSHGUI_EXPORT Color
		{
		public:
			union
			{
				struct
				{
					std::uint8_t B;
					std::uint8_t G;
					std::uint8_t R;
					std::uint8_t A;
				};
				std::uint32_t ARGB;
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
			Color(std::uint32_t argb);
			/**
			 * Legt eine Farbe mit den Werten f�r Rot, Gr�n und Blau an.
			 *
			 * @param red
			 * @param green
			 * @param blue
			 */
			Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue);
			/**
			 * Legt eine Farbe mit den Werten f�r Alpha, Rot, Gr�n und Blau an.
			 *
			 * @param alpha
			 * @param red
			 * @param green
			 * @param blue
			 */
			Color(std::uint8_t alpha, std::uint8_t red, std::uint8_t green, std::uint8_t blue);

			Color& operator+=(const Color &rhs);
			Color& operator-=(const Color &rhs);
			Color& operator*=(const Color &rhs);
			Color& operator*=(float rhs);

			/**
			 * Gibt eine leere Farbe (ARGB = 0) zur�ck.
		     */
			static Color Empty();

			/**
			 * Gibt die Farbe Rot zur�ck (255, 255, 0, 0)
			 */
			static Color Red();
			/**
			 * Gibt die Farbe Lime zur�ck (255, 0, 255, 0)
			 */
			static Color Lime();
			/**
			 * Gibt die Farbe Blau zur�ck (255, 0, 0, 255)
			 */
			static Color Blue();

			/**
			 * Gibt die Farbe Schwarz zur�ck (255, 0, 0, 0)
			 */
			static Color Black();
			/**
			 * Gibt die Farbe Grau zur�ck (255, 128, 128, 128)
			 */
			static Color Grey();
			/**
			 * Gibt die Farbe Wei� zur�ck (255, 255, 255, 255)
			 */
			static Color White();

			/**
			 * Gibt die Farbe Gelb zur�ck (255, 255, 255, 0)
			 */
			static Color Yellow();
			/**
			 * Gibt die Farbe Fuchsia zur�ck (255, 255, 0, 255)
			 */
			static Color Fuchsia();
			/**
			 * Gibt die Farbe Cyan zur�ck (255, 0, 255, 255)
			 */
			static Color Cyan();
			/**
			 * Gibt die Farbe Orange zur�ck (255, 255, 125, 0)
			 */
			static Color Orange();

			/**
			 * Gibt die Farbe Maroon zur�ck (255, 128, 0, 0)
			 */
			static Color Maroon();
			/**
			 * Gibt die Farbe Gr�n zur�ck (255, 0, 128, 0)
			 */
			static Color Green();
			/**
			 * Gibt die Farbe Navy zur�ck (255, 0, 0, 128)
			 */
			static Color Navy();

			/**
			 * Berechnet den Farbton der Farbe.
			 *
			 * @return der Farbton
			 */
			float Hue() const;
			/**
			 * Berechnet die S�ttigung der Farbe.
			 *
			 * @return die S�ttigung
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
			 * @param saturation die S�ttigugn
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