/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_COLOR_HPP
#define OSHGUI_DRAWING_COLOR_HPP

#include "../Exports.hpp"

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
					unsigned char B;
					unsigned char G;
					unsigned char R;
					unsigned char A;
				};
				unsigned long ARGB;
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
			Color(unsigned long argb);
			/**
			 * Legt eine Farbe mit den Werten für Rot, Grün und Blau an.
			 *
			 * @param red
			 * @param green
			 * @param blue
			 */
			Color(unsigned int red, unsigned int green, unsigned int blue);
			/**
			 * Legt eine Farbe mit den Werten für Alpha, Rot, Grün und Blau an.
			 *
			 * @param alpha
			 * @param red
			 * @param green
			 * @param blue
			 */
			Color(unsigned int alpha, unsigned int red, unsigned int green, unsigned int blue);

			bool operator == (const Color &color) const;
			bool operator != (const Color &color) const;
			Color operator + (const Color &color) const;
			Color& operator += (const Color &color);
			Color operator - (const Color &color) const;
			Color& operator -= (const Color &color);

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
	}
}

#endif