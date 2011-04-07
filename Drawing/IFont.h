#ifndef __OSHGUI_DRAWING_IFONT_H__
#define __OSHGUI_DRAWING_IFONT_H__

#include <windows.h>

#include "E:\Coding\OSHGui\Drawing\Point.h"
#include "E:\Coding\OSHGui\Drawing\Size.h"
#include "E:\Coding\OSHGui\Drawing\Rectangle.h"

namespace OSHGui
{
	namespace Drawing
	{
		class IFont
		{
		public:
			virtual bool Create(LPCSTR fontName, int size, bool bold, bool italic) = 0;
			virtual int MeasureCharacter(char c) = 0;
		};
	}
}

#endif