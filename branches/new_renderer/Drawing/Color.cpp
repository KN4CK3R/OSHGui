/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "Color.hpp"
#include <algorithm>

namespace OSHGui
{
	namespace Drawing
	{
		template<typename T>
		T clamp(const T &val, const T &min, const T &max)
		{
			return val < min ? min : val > max ? max : val;
		}

		argb_t ARGBCombine(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
		{
			return static_cast<argb_t>(alpha) << 24 |
				   static_cast<argb_t>(red) << 16 |
				   static_cast<argb_t>(green) << 8 |
				   static_cast<argb_t>(blue);
		}

		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Color::Color()
			: blue(0),
			  green(0),
			  red(0),
			  alpha(0),
			  argb(0)
		{

		}
		//---------------------------------------------------------------------------
		Color::Color(uint32_t argb)
		{
			SetARGB(argb);
		}
		//---------------------------------------------------------------------------
		Color::Color(float red, float green, float blue)
			: blue(blue),
			  green(green),
			  red(red),
			  alpha(1)
		{
			CalculateARGB();
		}
		//---------------------------------------------------------------------------
		Color::Color(float alpha, float red, float green, float blue)
			: blue(blue),
			  green(green),
			  red(red),
			  alpha(alpha < 0.f ? 0.f : alpha > 1.f ? 1.f : alpha)
		{
			CalculateARGB();
		}
		//---------------------------------------------------------------------------
		Color Color::FromRGB(uint8_t red, uint8_t green, uint8_t blue)
		{
			return FromARGB(255, red, green, blue);
		}
		//---------------------------------------------------------------------------
		Color Color::FromARGB(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
		{
			return Color(ARGBCombine(alpha, red, green, blue));
		}
		//---------------------------------------------------------------------------
		//predefined colors
		//---------------------------------------------------------------------------
		Color Color::Empty() { return Color(); }

		Color Color::Red() { return Color(1, 1, 0, 0); }
		Color Color::Lime() { return Color(1, 0, 1, 0); }
		Color Color::Blue() { return Color(1, 0, 0, 1); }

		Color Color::Black() { return Color(1, 0, 0, 0); }
		Color Color::Grey() { return Color(1, 0.5f, 0.5f, 0.5f); }
		Color Color::White() { return Color(1, 1, 1, 1); }

		Color Color::Yellow() { return Color(1, 1, 1, 0); }
		Color Color::Fuchsia() { return Color(1, 1, 0, 1); }
		Color Color::Cyan() { return Color(1, 0, 1, 1); }
		Color Color::Orange() { return Color(1, 1, 0.5f, 0); }

		Color Color::Maroon() { return Color(1, 0.5f, 0, 0); }
		Color Color::Green() { return Color(1, 0, 0.5f, 0); }
		Color Color::Navy() { return Color(1, 0, 0, 0.5f); }
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		float Color::GetRed() const
		{
			return red;
		}
		//---------------------------------------------------------------------------
		float Color::GetGreen() const
		{
			return green;
		}
		//---------------------------------------------------------------------------
		float Color::GetBlue() const
		{
			return blue;
		}
		//---------------------------------------------------------------------------
		float Color::GetAlpha() const
		{
			return alpha;
		}
		//---------------------------------------------------------------------------
		argb_t Color::GetARGB() const
		{
			return argb;
		}
		//---------------------------------------------------------------------------
		void Color::SetARGB(argb_t argb_)
		{
			argb = argb_;

			blue = static_cast<float>(argb_ & 0xFF) / 255.0f;
			argb_ >>= 8;
			green = static_cast<float>(argb_ & 0xFF) / 255.0f;
			argb_ >>= 8;
			red = static_cast<float>(argb_ & 0xFF) / 255.0f;
			argb_ >>= 8;
			alpha = static_cast<float>(argb_ & 0xFF) / 255.0f;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void Color::Clamp()
		{
			alpha = clamp(alpha, 0.f, 1.f);
			red = clamp(red, 0.f, 1.f);
			green = clamp(green, 0.f, 1.f);
			blue = clamp(blue, 0.f, 1.f);
			
			CalculateARGB();
		}
		//---------------------------------------------------------------------------
		Color Color::Clamped() const
		{
			auto copy(*this);
			copy.Clamp();
			return copy;
		}
		//---------------------------------------------------------------------------
		void Color::CalculateARGB()
		{
			argb = ARGBCombine(
				static_cast<uint8_t>((clamp(alpha, 0.f, 1.f) * 255.0f) + 0.5f),
				static_cast<uint8_t>((clamp(red, 0.f, 1.f) * 255.0f) + 0.5f),
				static_cast<uint8_t>((clamp(green, 0.f, 1.f) * 255.0f) + 0.5f),
				static_cast<uint8_t>((clamp(blue, 0.f, 1.f) * 255.0f) + 0.5f)
			);
		}
		//---------------------------------------------------------------------------
		float Color::Hue() const
		{
			if (red == green && green == blue)
			{
				return 0.0f;
			}

			float r = red;
			float g = green;
			float b = blue;

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
			float r = red;
			float g = green;
			float b = blue;

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
			float r = red;
			float g = green;
			float b = blue;
		 
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
				return Color(brightness, t, p);
			}
			else if (h < 2)
			{
				return Color(q, brightness, p);
			}
			else if (h < 3)
			{
				return Color(p, brightness, t);
			}
			else if (h < 4)
			{
				return Color(p, q, brightness);
			}
			else if (h < 5)
			{
				return Color(t, p, brightness);
			}
			else
			{
				return Color(brightness, p, q);
			}
		}
		//---------------------------------------------------------------------------
		bool operator==(const Color &lhs, const Color &rhs)
		{
			return lhs.red == rhs.red
				&& lhs.green == rhs.green
				&& lhs.blue == rhs.blue
				&& lhs.alpha == rhs.alpha;
		}
		//---------------------------------------------------------------------------
		bool operator!=(const Color &lhs, const Color &rhs)
		{
			return !(lhs == rhs);
		}
		//---------------------------------------------------------------------------
		const Color operator+(const Color &lhs, const Color &rhs)
		{
			return Color(lhs.alpha + rhs.alpha, lhs.red + rhs.red, lhs.green + rhs.green, lhs.blue + rhs.blue);
		}
		//---------------------------------------------------------------------------
		const Color operator-(const Color &lhs, const Color &rhs)
		{
			return Color(lhs.alpha - rhs.alpha, lhs.red - rhs.red, lhs.green - rhs.green, lhs.blue - rhs.blue);
		}
		//---------------------------------------------------------------------------
		const Color operator*(const Color &lhs, const Color &rhs)
		{
			return Color(lhs.alpha * rhs.alpha, lhs.red * rhs.red, lhs.green * rhs.green, lhs.blue * rhs.blue);
		}
		//---------------------------------------------------------------------------
		const Color operator*(const Color &lhs, float rhs)
		{
			return Color(lhs.alpha * rhs, lhs.red * rhs, lhs.green * rhs, lhs.blue * rhs);
		}
		//---------------------------------------------------------------------------
	}
}