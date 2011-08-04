#include "TextHelper.h"

namespace OSHGui
{
	namespace Misc
	{
		TextHelper::TextHelper(Drawing::IFont *font)
		{
			SetFont(font);
		}
		//---------------------------------------------------------------------------
		void TextHelper::SetFont(Drawing::IFont *font)
		{
			if (font == NULL)
			{
				throw "Font can't be null.";
			}
		
			this->font = font;
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		void TextHelper::SetText(const UnicodeString &text)
		{
			this->text = text;
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		int TextHelper::GetLength()
		{
			return text.length();
		}
		//---------------------------------------------------------------------------
		const UnicodeString& TextHelper::GetText()
		{
			return text;
		}
		//---------------------------------------------------------------------------
		const Drawing::Size& TextHelper::GetSize()
		{
			return size;
		}
		//---------------------------------------------------------------------------
		void TextHelper::RefreshSize()
		{
			size = font->MeasureText(text);
		}
		//---------------------------------------------------------------------------
		Drawing::Point TextHelper::GetCharacterPosition(int index)
		{
			if (GetLength() == 0 || index == 0)
			{
				return Drawing::Point(1, 0);
			}
			
			UnicodeString substring = text.substr(0, index);
			Drawing::Size size = font->MeasureText(substring);
			
			return Drawing::Point(size.Width, size.Height >= font->GetSize() ? size.Height - font->GetSize() : size.Height);
		}
		//---------------------------------------------------------------------------
		int TextHelper::GetClosestCharacterIndex(Drawing::Point position)
		{
			int distance = 0xFFFF;
			int result;

			for (int i = 0; i <= text.length(); ++i)
			{
				Drawing::Point charPosition = GetCharacterPosition(i);

				int actualDistance = abs(charPosition.Left - position.Left);

				if (actualDistance > distance)
				{
					break;
				}

				distance = actualDistance;
				result = i;
			}

			return result;
		}
		//---------------------------------------------------------------------------
	}
}