#ifndef __OSHGUI_DRAWING_COLOR_H__
#define __OSHGUI_DRAWING_COLOR_H__

#include <windows.h>

namespace OSHGui
{
	namespace Drawing
	{
		class Color
		{
		public:
			enum ColorFormat
			{
				COLOR_ARGB, COLOR_RGBA, COLOR_RGB, COLOR_ABGR, COLOR_BGRA, COLOR_BGR
			};

			union
			{
				struct
				{
					BYTE B;
					BYTE G;
					BYTE R;
					BYTE A;
				};
				unsigned long ARGB;
			};

			Color();
			Color(unsigned long ARGB);
			Color(unsigned int red, unsigned int green, unsigned int blue);
			Color(unsigned int alpha, unsigned int red, unsigned int green, unsigned int blue);

			Color operator + (const Color &color) const;
			void operator += (const Color &color);
			Color operator - (const Color &color) const;
			void operator -= (const Color &color);

			static Color Empty();

			static Color Red();
			static Color Lime();
			static Color Blue();

			static Color Black();
			static Color Grey();
			static Color White();

			static Color Yellow();
			static Color Fuchsia();
			static Color Cyan();

			static Color Maroon();
			static Color Green();
			static Color Navy();

			float Hue();
			float Saturation();
			float Brightness();

			static Color FromHSB(float hue, float saturation, float brightness);

			DWORD Format(ColorFormat format);
		};
	}
}

#endif