#include "TextHelper.h"

namespace OSHGui
{
	namespace Misc
	{
		TextHelper::TextHelper(Drawing::IFont *font)
		{
			SetFont(font);
		}
		
		void TextHelper::SetFont(Drawing::IFont *font)
		{
			if (font == NULL)
			{
				throw "Font can't be null.";
			}
		
			this->font = font;
		}
		
		void TextHelper::SetText(UnicodeString &text)
		{
			this->text = text;
		}
		
		int TextHelper::GetLength()
		{
			return text.length();
		}
		
		const UnicodeString& TextHelper::GetText()
		{
			return text;
		}
		
		void TextHelper::RefreshSize()
		{
			
		}
		
		Drawing::Point TextHelper::GetCharacterPosition(int index)
		{
			if (GetLength() == 0 || index == 0)
			{
				return Drawing::Point(1, 0);
			}
			
			UnicodeString substring = text.substr(0, index);
			Drawing::Size size = font->MeasureText(substring);
			
			if ( p.y >= m_Font->size )
				p.y -= m_Font->size;

			return Drawing::Point(size.Width, size.Height >= font->GetSize() ? size.Height - font->GetSize() : size.Height);
		}
		
		int TextHelper::GetClosestCharacterIndex(Drawing::Point position)
		{
		
		}
	}
}