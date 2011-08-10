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
			void Append(const UnicodeChar character);
			void Append(const UnicodeString &text);
			void Insert(int position, const UnicodeChar character);
			void Insert(int position, const UnicodeString &text);
			void Clear();
			void Remove(int index, int length = 1);
			
			int GetLength();
			const UnicodeString& GetText();
			const Drawing::Size& GetSize();
			
			void RefreshSize();
			
			Drawing::Point GetCharacterPosition(int index, bool trailing = false);
			Drawing::Size GetStringWidth(int index, int size = -1);
			int GetClosestCharacterIndex(const Drawing::Point &position);
			
		private:
			UnicodeString text;
			
			Drawing::Size size;
			
			Drawing::IFont *font;
		};
	}
}

#endif