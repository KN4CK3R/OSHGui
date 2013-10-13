/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Color.hpp"
#include <algorithm>

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Color::Color()
			: B(0),
			  G(0),
			  R(0),
			  A(0)
		{

		}
		//---------------------------------------------------------------------------
		Color::Color(std::uint32_t argb)
			: ARGB(argb)
		{

		}
		//---------------------------------------------------------------------------
		Color::Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
			: B(blue),
			  G(green),
			  R(red),
			  A(255)
		{

		}
		//---------------------------------------------------------------------------
		Color::Color(std::uint8_t alpha, std::uint8_t red, std::uint8_t green, std::uint8_t blue)
			: B(blue),
			  G(green),
			  R(red),
			  A(alpha)
		{
			
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
		Color& Color::operator+=(const Color &rhs)
		{
			A = std::max(0, std::min(255, A + rhs.A));
			R = std::max(0, std::min(255, R + rhs.R));
			G = std::max(0, std::min(255, G + rhs.G));
			B = std::max(0, std::min(255, B + rhs.B));

			return *this;
		}
		//---------------------------------------------------------------------------
		Color& Color::operator-=(const Color &rhs)
		{
			A = std::max(0, std::min(255, A - rhs.A));
			R = std::max(0, std::min(255, R - rhs.R));
			G = std::max(0, std::min(255, G - rhs.G));
			B = std::max(0, std::min(255, B - rhs.B));

			return *this;
		}
		//---------------------------------------------------------------------------
		Color& Color::operator*=(const Color &rhs)
		{
			A = std::max(0, std::min(255, A * rhs.A));
			R = std::max(0, std::min(255, R * rhs.R));
			G = std::max(0, std::min(255, G * rhs.G));
			B = std::max(0, std::min(255, B * rhs.B));

			return *this;
		}
		//---------------------------------------------------------------------------
		Color& Color::operator*=(float rhs)
		{
			A = std::max(0, std::min(255, (int)(A * rhs)));
			R = std::max(0, std::min(255, (int)(R * rhs)));
			G = std::max(0, std::min(255, (int)(G * rhs)));
			B = std::max(0, std::min(255, (int)(B * rhs)));

			return *this;
		}
		//---------------------------------------------------------------------------
		float Color::Hue() const
		{
			if (R == G && G == B)
			{
				return 0.0f;
			}

			float r = R / 255.0f; 
			float g = G / 255.0f;
			float b = B / 255.0f; 

			float max = r > g ? r : g > b ? g : b;
			float min = r < g ? r : g < b ? g : b;
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
		float Color::Saturation() const
		{
			float r = R / 255.0f;
			float g = G / 255.0f;
			float b = B / 255.0f;

			float max = r > g ? r : g > b ? g : b;
			float min = r < g ? r : g < b ? g : b;
			float s = 0;

			if (max != min)
			{
				float l = (max + min) / 2;

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
		float Color::Brightness() const
		{
			float r = R / 255.0f;
			float g = G / 255.0f;
			float b = B / 255.0f;
		 
			float max = r > g ? r : g > b ? g : b;
			float min = r < g ? r : g < b ? g : b;

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
				return Color(brightness * 255, t * 255, p * 255);
			}
			else if (h < 2)
			{
				return Color(q * 255, brightness * 255, p * 255);
			}
			else if (h < 3)
			{
				return Color(p * 255, brightness * 255, t * 255);
			}
			else if (h < 4)
			{
				return Color(p * 255, q * 255, brightness * 255);
			}
			else if (h < 5)
			{
				return Color(t * 255, p * 255, brightness * 255);
			}
			else
			{
				return Color(brightness * 255, p * 255, q * 255);
			}
		}
		//---------------------------------------------------------------------------
		bool operator==(const Color &lhs, const Color &rhs)
		{
			return lhs.ARGB == rhs.ARGB;
		}
		//---------------------------------------------------------------------------
		bool operator!=(const Color &lhs, const Color &rhs)
		{
			return !(lhs == rhs);
		}
		//---------------------------------------------------------------------------
		const Color operator+(const Color &lhs, const Color &rhs)
		{
			auto temp(lhs);
			temp += rhs;
			return temp;
		}
		//---------------------------------------------------------------------------
		const Color operator-(const Color &lhs, const Color &rhs)
		{
			auto temp(lhs);
			temp -= rhs;
			return temp;
		}
		//---------------------------------------------------------------------------
		const Color operator*(const Color &lhs, const Color &rhs)
		{
			auto temp(lhs);
			temp *= rhs;
			return temp;
		}
		//---------------------------------------------------------------------------
		const Color operator*(const Color &lhs, float rhs)
		{
			auto temp(lhs);
			temp *= rhs;
			return temp;
		}
		//---------------------------------------------------------------------------
	}
}