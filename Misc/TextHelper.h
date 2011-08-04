#ifndef __OSHGUI_MISC_TEXTHELPER_H__
#define __OSHGUI_MISC_TEXTHELPER_H__

#include "Misc\Strings.h"
#include "Drawing\IFont.h"
#include "Drawing\Point.h"
#include "Drawing\Size.h"

namespace OSHGui
{
	namespace Misc
	{
		class TextHelper
		{
		public:
			TextHelper(Drawing::IFont *font);
			
			void SetFont(Drawing::IFont *font);
			void SetText(const UnicodeString &text);
			
			int GetLength();
			const UnicodeString& GetText();
			const Drawing::Size& GetSize();
			
			void RefreshSize();
			
			Drawing::Point GetCharacterPosition(int index);
			int GetClosestCharacterIndex(Drawing::Point position);
			
		private:
			UnicodeString text;
			
			Drawing::Size size;
			
			Drawing::IFont *font;
		};
	}
}

#endif