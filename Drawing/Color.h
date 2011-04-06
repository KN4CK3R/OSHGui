#ifndef __OSHGUI_DRAWING_COLOR_H__
#define __OSHGUI_DRAWING_COLOR_H__

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
					BYTE Alpha;
					BYTE Red;
					BYTE Green;
					BYTE Blue;
				}
				DWORD Color;
			};
		
			//---------------------------------------------------------------------------
			//Constructor
			//---------------------------------------------------------------------------
			Color(BYTE alpha = 0, BYTE red = 0, BYTE green = 0, BYTE blue = 0)
			{
				Alpha = alpha;
				Red = red;
				Green = green;
				Blue = blue;
			}
			//---------------------------------------------------------------------------
			//predefined colors
			//---------------------------------------------------------------------------
			static Color Empty;
		};
	}
}

#endif