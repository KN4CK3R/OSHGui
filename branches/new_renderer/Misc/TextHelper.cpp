/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "TextHelper.hpp"
#include "Exceptions.hpp"

namespace OSHGui
{
	namespace Misc
	{
		TextHelper::TextHelper(const Drawing::FontPtr &font)
		{
			SetFont(font);
		}
		//---------------------------------------------------------------------------
		void TextHelper::SetFont(const Drawing::FontPtr &font)
		{
			if (font == nullptr)
			{
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw ArgumentNullException("font");
				#endif
				return;
			}
		
			this->font = font;
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		void TextHelper::SetText(const AnsiString &text)
		{
			this->text = text;
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		void TextHelper::Append(const AnsiChar character)
		{
			text.append(1, character);
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		void TextHelper::Append(const AnsiString &text)
		{
			this->text.append(text);
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		void TextHelper::Insert(int position, const AnsiChar character)
		{
			text.insert(position, 1, character);
			RefreshSize();
		}
		//---------------------------------------------------------------------------
		void TextHelper::Insert(int position, const AnsiString &_text)
		{
			text.insert(position, _text);
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
			if (index >= text.length())
			{
				return;
			}
			if (index + length > (int)text.length())
			{
				length = text.length() - index;
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
		const AnsiString& TextHelper::GetText() const
		{
			return text;
		}
		//---------------------------------------------------------------------------
		const Drawing::SizeF& TextHelper::GetSize() const
		{
			return size;
		}
		//---------------------------------------------------------------------------
		void TextHelper::RefreshSize()
		{
			size = GetStringWidth(0);
		}
		//---------------------------------------------------------------------------
		Drawing::PointF TextHelper::GetCharacterPosition(int index, bool trailing) const
		{
			if (GetLength() == 0)
			{
				return Drawing::PointF(0, 0);
			}
			if (index == 0)
			{
				if (!trailing)
				{
					return Drawing::PointF(0, 0);
				}
			}
			
			auto substring = text.substr(0, trailing ? index + 1 : index);
			Drawing::SizeF size(font->GetTextExtent(substring), font->GetFontHeight());
			
			return Drawing::PointF(size.Width, size.Height);//Drawing::PointF(size.Width - 2, size.Height < font->GetSize() ? font->GetSize() : size.Height);
		}
		//---------------------------------------------------------------------------
		Drawing::SizeF TextHelper::GetStringWidth(int index, int size) const
		{
			if (GetLength() == 0 || size == 0)
			{
				return Drawing::SizeF(0, font->GetFontHeight());
			}
			if (index >= GetLength())
			{
				index = GetLength() - 1;
			}

			auto substring = size == -1 ? text.substr(index) : text.substr(index, size);
			return Drawing::SizeF(font->GetTextExtent(substring), font->GetFontHeight());
		}
		//---------------------------------------------------------------------------
		int TextHelper::GetClosestCharacterIndex(const Drawing::PointF &position) const
		{
			int distance = 0xFFFF;
			int result = 0;

			if (position.Left >= size.Width)
			{
				return text.length() + 1;
			}

			for (unsigned int i = 0; i < text.length(); ++i)
			{
				Drawing::PointF charPosition = GetCharacterPosition(i);

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