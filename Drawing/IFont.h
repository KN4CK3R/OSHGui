#ifndef __OSHGUI_DRAWING_IFONT_H__
#define __OSHGUI_DRAWING_IFONT_H__

#include <windows.h>

#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\Point.h"
#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\Size.h"
#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\Rectangle.h"

#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = NULL; } }
#endif

namespace OSHGui
{
	namespace Drawing
	{
		class IFont
		{
		public:
			virtual bool Create(LPCWSTR fontName, int size, bool bold, bool italic) = 0;
		};
	}
}

#endif