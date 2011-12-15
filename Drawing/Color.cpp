#include "Color.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Color::Color()
		{
			A = 0;
			R = 0;
			G = 0;
			B = 0;
		}
		//---------------------------------------------------------------------------
		Color::Color(unsigned long argb)
		{
			ARGB = argb;
		}
		//---------------------------------------------------------------------------
		Color::Color(unsigned int red, unsigned int green, unsigned int blue)
		{
			A = 255;
			R = red & 0xFF;
			G = green & 0xFF;
			B = blue & 0xFF;
		}
		//---------------------------------------------------------------------------
		Color::Color(unsigned int alpha, unsigned int red, unsigned int green, unsigned int blue)
		{
			A = (alpha > 255 ? 255 : alpha) & 0xFF;
			R = (red > 255 ? 255 : red) & 0xFF;
			G = (green > 255 ? 255 : green) & 0xFF;
			B = (blue > 255 ? 255 : blue) & 0xFF;
		}
		//---------------------------------------------------------------------------
		//predefined colors
		//---------------------------------------------------------------------------
		Color Color::Empty() { return Color(); }

		Color Color::Red() { return Color(255, 255, 0, 0); }
		Color Color::Lime() { return Color(255, 0, 255, 0); }
		Color Color::Blue() { return Color(255, 0, 0, 255); }

		Color Color::Black() { return Color(255, 0, 0, 0); }
		Color Color::Grey() { return Color(255, 128, 128, 128); }
		Color Color::White() { return Color(255, 255, 255, 255); }

		Color Color::Yellow() { return Color(255, 255, 255, 0); }
		Color Color::Fuchsia() { return Color(255, 255, 0, 255); }
		Color Color::Cyan() { return Color(255, 0, 255, 255); }
		Color Color::Orange() { return Color(255, 255, 125, 0); }

		Color Color::Maroon() { return Color(255, 128, 0, 0); }
		Color Color::Green() { return Color(255, 0, 128, 0); }
		Color Color::Navy() { return Color(255, 0, 0, 128); }
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		bool Color::operator == (const Color &color) const
		{
			return ARGB == color.ARGB;
		}
		//---------------------------------------------------------------------------
		bool Color::operator != (const Color &color) const
		{
			return ARGB != color.ARGB;
		}
		//---------------------------------------------------------------------------
		Color Color::operator + (const Color &color) const
		{
			unsigned int alpha = A + color.A;
			alpha = alpha > 255 ? 255 : alpha ;
			unsigned int red = R + color.R;
			red = red > 255 ? 255 :red;
			unsigned int green = G + color.G;
			green = green > 255 ? 255 : green;
			unsigned int blue = B + color.B;
			blue = blue > 255 ? 255 : blue;

			return Color(alpha, red, green, blue);
		}
		//---------------------------------------------------------------------------
		Color& Color::operator += (const Color &color)
		{
			unsigned int alpha = A + color.A;
			A = (alpha > 255 ? 255 : alpha) & 0xFF;
			unsigned int red = R + color.R;
			R = (red > 255 ? 255 : red) & 0xFF;
			unsigned int green = G + color.G;
			G = (green > 255 ? 255 : green) & 0xFF;
			unsigned int blue = B + color.B;
			B = (blue > 255 ? 255 : blue) & 0xFF;

			return *this;
		}
		//---------------------------------------------------------------------------
		Color Color::operator - (const Color &color) const
		{
			int alpha = A - color.A;
			alpha = alpha < 0 ? 0 : alpha ;
			int red = R - color.R;
			red = red < 0 ? 0 :red;
			int green = G - color.G;
			green = green < 0 ? 0 : green;
			int blue = B - color.B;
			blue = blue < 0 ? 0 : blue;

			return Color(alpha, red, green, blue);
		}
		//---------------------------------------------------------------------------
		Color& Color::operator -= (const Color &color)
		{
			int alpha = A - color.A;
			A = (alpha < 0 ? 0 : alpha) & 0xFF;
			int red = R - color.R;
			R = (red < 0 ? 0 : red) & 0xFF;
			int green = G - color.G;
			G = (green < 0 ? 0 : green) & 0xFF;
			int blue = B - color.B;
			B = (blue < 0 ? 0 : blue) & 0xFF;

			return *this;
		}
		//---------------------------------------------------------------------------
		float Color::Hue()
		{
			if (R == G && G == B)
			{
				return 0.0f;
			}

			float r = R / 255.0f; 
			float g = G / 255.0f;
			float b = B / 255.0f; 

			float max = r > g ? r : g > b ? g : b,
				  min = r < g ? r : g < b ? g : b;
			float delta = max - min; 
			float hue = 0.0f;

			if (r == max)
			{
				hue = (g - b) / delta;
			}
			else if (g == max)
			{
				hue = 2 + (b - r) / delta;
			}
			else if (b == max)
			{
				hue = 4 + (r - g) / delta;
			}
			hue *= 60;

			if (hue < 0.0f)
			{
				hue += 360.0f;
			}
			return hue;
		}
		//---------------------------------------------------------------------------
		float Color::Saturation()
		{
			float r = R / 255.0f;
			float g = G / 255.0f;
			float b = B / 255.0f;

			float max = r > g ? r : g > b ? g : b,
				  min = r < g ? r : g < b ? g : b,
				  l,
				  s = 0;

			if (max != min)
			{
				l = (max + min) / 2;

				if (l <= 0.5f)
				{
					s = (max - min) / (max + min);
				}
				else
				{
					s = (max - min) / (2 - max - min);
				}
			}
			
			return s;
		}
		//---------------------------------------------------------------------------
		float Color::Brightness()
		{
			float r = R / 255.0f;
			float g = G / 255.0f;
			float b = B / 255.0f;
		 
			float max = r > g ? r : g > b ? g : b,
				  min = r < g ? r : g < b ? g : b;

			return (max + min) / 2;
		}
		//---------------------------------------------------------------------------
		Color Color::FromHSB(float hue, float saturation, float brightness)
		{
			float h = hue == 1.0f ? 0 : hue * 6.0f;
			float f = h - (int)h;
			float p = brightness * (1.0f - saturation);
			float q = brightness * (1.0f - saturation * f);
			float t = brightness * (1.0f - (saturation * (1.0f - f)));

			if (h < 1)
			{
				return Color(
							(unsigned char)(brightness * 255),
							(unsigned char)(t * 255),
							(unsigned char)(p * 255)
							);
			}
			else if (h < 2)
			{
				return Color(
							(unsigned char)(q * 255),
							(unsigned char)(brightness * 255),
							(unsigned char)(p * 255)
							);
			}
			else if (h < 3)
			{
				return Color(
							(unsigned char)(p * 255),
							(unsigned char)(brightness * 255),
							(unsigned char)(t * 255)
							);
			}
			else if (h < 4)
			{
				return Color(
							(unsigned char)(p * 255),
							(unsigned char)(q * 255),
							(unsigned char)(brightness * 255)
							);
			}
			else if (h < 5)
			{
				return Color(
							(unsigned char)(t * 255),
							(unsigned char)(p * 255),
							(unsigned char)(brightness * 255)
							);
			}
			else
			{
				return Color(
							(unsigned char)(brightness * 255),
							(unsigned char)(p * 255),
							(unsigned char)(q * 255)
							);
			}
		}
		//---------------------------------------------------------------------------
	}
}