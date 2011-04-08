#ifndef __OSHGUI_DRAWING_IFONT_H__
#define __OSHGUI_DRAWING_IFONT_H__

#include "Point.h"
#include "Size.h"
#include "Rectangle.h"

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