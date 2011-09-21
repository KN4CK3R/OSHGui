#include "TextHelper.h"

namespace OSHGui
{
	namespace Misc
	{
		TextHelper::TextHelper(const std::shared_ptr<Drawing::IFont> &font)
		{
			SetFont(font);
		}
		//---------------------------------------------------------------------------
		void TextHelper::SetFont(const std::shared_ptr<Drawing::IFont> &font)
		{
			if (font == 0)
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
		void TextHelper::Append(const UnicodeChar character)
		{
			text.append(1, character);
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		void TextHelper::Append(const UnicodeString &text)
		{
			this->text.append(text);
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		void TextHelper::Insert(int position, const UnicodeChar character)
		{
			text.insert(position, 1, character);
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		void TextHelper::Insert(int position, const UnicodeString &text)
		{
			this->text.insert(position, text);
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		void TextHelper::Clear()
		{
			text.clear();
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		void TextHelper::Remove(int index, int length)
		{
			if (index + length > (int)text.length())
			{
				index = index;
			}
			text.erase(index, length);
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		int TextHelper::GetLength() const
		{
			return text.length();
		}
		//---------------------------------------------------------------------------
		const UnicodeString& TextHelper::GetText() const
		{
			return text;
		}
		//---------------------------------------------------------------------------
		const Drawing::Size& TextHelper::GetSize() const
		{
			return size;
		}
		//---------------------------------------------------------------------------
		void TextHelper::RefreshSize()
		{
			size = GetStringWidth(0);
		}
		//---------------------------------------------------------------------------
		Drawing::Point TextHelper::GetCharacterPosition(int index, bool trailing) const
		{
			if (GetLength() == 0)
			{
				return Drawing::Point(0, 0);
			}
			if (index == 0)
			{
				if (!trailing)
				{
					return Drawing::Point(0, 0);
				}
			}
			
			UnicodeString substring = text.substr(0, trailing ? index + 1 : index);
			Drawing::Size size = font->MeasureText(substring);
			
			return Drawing::Point(size.Width - 2, size.Height < font->GetSize() ? font->GetSize() : size.Height);
		}
		//---------------------------------------------------------------------------
		Drawing::Size TextHelper::GetStringWidth(int index, int size) const
		{
			if (GetLength() == 0 || size == 0)
			{
				return Drawing::Size(0, font->GetSize());
			}
			if (index >= GetLength())
			{
				index = GetLength() - 1;
			}

			UnicodeString substring = size == -1 ? text.substr(index) : text.substr(index, size);
			return font->MeasureText(substring);
		}
		//---------------------------------------------------------------------------
		int TextHelper::GetClosestCharacterIndex(const Drawing::Point &position) const
		{
			int distance = 0xFFFF;
			int result = 0;

			if (position.Left >= size.Width)
			{
				return text.length() + 1;
			}

			for (unsigned int i = 0; i < text.length(); ++i)
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