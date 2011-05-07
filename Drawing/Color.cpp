#include "Color.h"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Color::Color()
		{
			Color(255, 0, 0, 0);
		}
		//---------------------------------------------------------------------------
		Color::Color(DWORD argb)
		{
			ARGB = argb;
		}
		//---------------------------------------------------------------------------
		Color::Color(unsigned int red, unsigned int green, unsigned int blue)
		{
			Color(255, red, green, blue);
		}
		//---------------------------------------------------------------------------
		Color::Color(unsigned int alpha, unsigned int red, unsigned int green, unsigned int blue)
		{
			A = alpha & 0xFF;
			R = red & 0xFF;
			G = green & 0xFF;
			B = blue & 0xFF;
		}
		//---------------------------------------------------------------------------
		//predefined colors
		//---------------------------------------------------------------------------
		Color Color::Empty() { return Color(0, 0, 0, 0); }

		Color Color::Red() { return Color(255, 255, 0, 0); }
		Color Color::Lime() { return Color(255, 0, 255, 0); }
		Color Color::Blue() { return Color(255, 0, 0, 255); }

		Color Color::Black() { return Color(255, 0, 0, 0); }
		Color Color::Grey() { return Color(255, 128, 128, 128); }
		Color Color::White() { return Color(255, 255, 255, 255); }

		Color Color::Yellow() { return Color(255, 255, 255, 0); }
		Color Color::Fuchsia() { return Color(255, 255, 0, 255); }
		Color Color::Cyan() { return Color(255, 0, 255, 255); }

		Color Color::Maroon() { return Color(255, 128, 0, 0); }
		Color Color::Green() { return Color(255, 0, 128, 0); }
		Color Color::Navy() { return Color(255, 0, 0, 128); }
		//---------------------------------------------------------------------------
		//Runtime-Functions
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

			if(h < 1)
			{
				return Color(
							(BYTE)(brightness * 255),
							(BYTE)(t * 255),
							(BYTE)(p * 255)
							);
			}
			else if(h < 2)
			{
				return Color(
							(BYTE)(q * 255),
							(BYTE)(brightness * 255),
							(BYTE)(p * 255)
							);
			}
			else if(h < 3)
			{
				return Color(
							(BYTE)(p * 255),
							(BYTE)(brightness * 255),
							(BYTE)(t * 255)
							);
			}
			else if(h < 4)
			{
				return Color(
							(BYTE)(p * 255),
							(BYTE)(q * 255),
							(BYTE)(brightness * 255)
							);
			}
			else if(h < 5)
			{
				return Color(
							(BYTE)(t * 255),
							(BYTE)(p * 255),
							(BYTE)(brightness * 255)
							);
			}
			else
			{
				return Color(
							(BYTE)(brightness * 255),
							(BYTE)(p * 255),
							(BYTE)(q * 255)
							);
			}
		}
		//---------------------------------------------------------------------------
		DWORD Color::Format(ColorFormat format)
		{
			switch (format)
			{
			case ColorFormat::COLOR_RGBA:


			case ColorFormat::COLOR_ARGB:
			case ColorFormat::COLOR_RGB:
			default:
				return ARGB;
			}
		}
		//---------------------------------------------------------------------------
	}
}