/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
		typedef uint32_t argb_t;

		/**
		 * Stellt eine ARGB-Farbe (Alpha, Rot, Gr�n, Blau) dar.
		 */
		class OSHGUI_EXPORT Color
		{
		public:
			/**
			 * Legt eine leere Farbe an (ARGB = 0)
			 */
			Color();
			/**
			 * Legt eine Farbe mit dem angegeben ARGB Wert an.
			 *
			 * \param argb
			 */
			Color(argb_t argb);
			/**
			 * Legt eine Farbe mit den Werten f�r Rot, Gr�n und Blau an.
			 *
			 * \param red
			 * \param green
			 * \param blue
			 */
			Color(float red, float green, float blue);
			/**
			 * Legt eine Farbe mit den Werten f�r Alpha, Rot, Gr�n und Blau an.
			 *
			 * \param alpha
			 * \param red
			 * \param green
			 * \param blue
			 */
			Color(float alpha, float red, float green, float blue);

			static Color FromRGB(uint8_t red, uint8_t green, uint8_t blue);
			static Color FromARGB(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);

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

			float GetRed() const;
			float GetGreen() const;
			float GetBlue() const;
			float GetAlpha() const;
			argb_t GetARGB() const;

			/**
			 * Berechnet den Farbton der Farbe.
			 *
			 * \return der Farbton
			 */
			float Hue() const;
			/**
			 * Berechnet die S�ttigung der Farbe.
			 *
			 * \return die S�ttigung
			 */
			float Saturation() const;
			/**
			 * Berechnet die Helligkeit der Farbe.
			 *
			 * \return die Helligkeit
			 */
			float Brightness() const;

			/**
			 * Wandelt HSB Farbinformationen in ein Color-Objekt um.
			 *
			 * \param hue der Farbton
			 * \param saturation die S�ttigugn
			 * \param brightness die Helligkeit
			 * \return color
			 */
			static Color FromHSB(float hue, float saturation, float brightness);

			friend bool operator==(const Color &lhs, const Color &rhs);
			friend const Color operator+(const Color &lhs, const Color &rhs);
			friend const Color operator-(const Color &lhs, const Color &rhs);
			friend const Color operator*(const Color &lhs, const Color &rhs);
			friend const Color operator*(const Color &lhs, float rhs);

		private:
			void SetARGB(argb_t argb);
			void CalculateARGB();

			float alpha;
			float red;
			float green;
			float blue;

			argb_t argb;
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