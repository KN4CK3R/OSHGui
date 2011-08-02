#ifndef __OSHGUI_DRAWING_IFONT_H__
#define __OSHGUI_DRAWING_IFONT_H__

#include "Point.h"
#include "Size.h"
#include "Rectangle.h"
#include "..\Misc\Strings.h"

namespace OSHGui
{
	namespace Drawing
	{
		class IFont
		{
		public:
			virtual bool Create(const String &fontName, int size, bool bold, bool italic) = 0;
			
			String& GetName() { return fontName; }
			int GetSize() { return size; }
			bool IsBold() { return bold; }
			bool IsItalic() { return italic; }
			
			virtual Size MeasureText(const String &string);
						
		protected:
			String fontName;
			int size;
			bool bold;
			bool italic;
		};
	}
}

#endif