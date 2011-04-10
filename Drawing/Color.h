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
			Color(BYTE red, BYTE green, BYTE blue);
			Color(BYTE alpha, BYTE red, BYTE green, BYTE blue);

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
		};
	}
}

#endif