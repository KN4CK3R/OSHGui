/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "TextBox.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::Size TextBox::DefaultSize(100, 24);
	const Drawing::Point TextBox::DefaultTextOffset(7, 5);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TextBox::TextBox()
		: textHelper(font)
	{
		type = CONTROL_TEXTBOX;

		SetSize(DefaultSize);

		blinkTime = Misc::TimeSpan::FromMilliseconds(500);
		nextBlinkTime = Application::Instance()->GetNow();

		firstVisibleCharacter = 0;
		caretPosition = 0;
		passwordChar = '\0';
		
		cursor = Cursors::Get(Cursors::IBeam);

		SetBackColor(Drawing::Color(0xFF242321));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	TextBox::~TextBox()
	{

	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TextBox::SetSize(const Drawing::Size &size)
	{
		Drawing::Size fixxed(size.Width, font->GetSize() + DefaultTextOffset.Top * 2);

		Control::SetSize(fixxed);

		textRect = Drawing::Rectangle(absoluteLocation.Left + DefaultTextOffset.Left, absoluteLocation.Top + DefaultTextOffset.Top, GetWidth() - DefaultTextOffset.Left * 2, GetHeight() - DefaultTextOffset.Top * 2);
	}
	//---------------------------------------------------------------------------
	void TextBox::SetFont(const std::shared_ptr<Drawing::IFont> &font)
	{
		Control::SetFont(font);

		SetSize(GetWidth(), 0);
	}
	//---------------------------------------------------------------------------
	void TextBox::SetText(const Misc::AnsiString &text)
	{
		realtext = text;
		if (passwordChar == '\0')
		{
			textHelper.SetText(text);
		}
		else
		{
			textHelper.SetText(Misc::AnsiString(text.length(), passwordChar));
		}

		PlaceCaret(text.length());
		
		textChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& TextBox::GetText() const
	{
		return passwordChar == '\0' ? textHelper.GetText() : realtext;
	}
	//---------------------------------------------------------------------------
	void TextBox::SetPasswordChar(const Misc::AnsiChar &passwordChar)
	{
		this->passwordChar = passwordChar;
		SetText(realtext);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiChar& TextBox::GetPasswordChar() const
	{
		return passwordChar;
	}
	//---------------------------------------------------------------------------
	TextChangedEvent& TextBox::GetTextChangedEvent()
	{
		return textChangedEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool TextBox::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void TextBox::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();
		
		textRect = Drawing::Rectangle(absoluteLocation.Left + DefaultTextOffset.Left, absoluteLocation.Top + DefaultTextOffset.Top, GetWidth() - DefaultTextOffset.Left * 2, GetHeight() - DefaultTextOffset.Top * 2);
		PlaceCaret(caretPosition);
	}
	//---------------------------------------------------------------------------
	void TextBox::ResetCaretBlink()
	{
		showCaret = true;
		nextBlinkTime = Application::Instance()->GetNow().Add(blinkTime);
	}
	//---------------------------------------------------------------------------
	void TextBox::PlaceCaret(int position)
	{
		if (position < 0)
		{
			position = 0;
		}
		caretPosition = position;

		Drawing::Point caretPositionTrail;
		Drawing::Point firstVisibleCharacterPosition = textHelper.GetCharacterPosition(firstVisibleCharacter);
		Drawing::Point newCaretPosition = textHelper.GetCharacterPosition(position);

		//if the new caretPosition is bigger than the text length
		if (position > textHelper.GetLength())
		{
			caretPosition = position = textHelper.GetLength();
			caretPositionTrail = newCaretPosition;
		}
		else
		{
			caretPositionTrail = textHelper.GetCharacterPosition(position, true);
		}

		//if the new caretPosition is smaller than the textRect
		if (newCaretPosition.Left <= firstVisibleCharacterPosition.Left)
		{
			if (position > 1)
			{
				firstVisibleCharacter = position - 2;
			}
			else
			{
				firstVisibleCharacter = position;
			}
		}
		else if (caretPositionTrail.Left > firstVisibleCharacterPosition.Left + textRect.GetWidth()) //if the new caretPosition is bigger than the textRect
		{
			int newFirstVisibleCharacterPositionLeft = caretPositionTrail.Left - textRect.GetWidth();
			int newFirstVisibleCharacter = textHelper.GetClosestCharacterIndex(Drawing::Point(newFirstVisibleCharacterPositionLeft, 0));

			Drawing::Point newFirstVisibleCharacterPosition = textHelper.GetCharacterPosition(newFirstVisibleCharacter);
			if (newFirstVisibleCharacterPosition.Left < newFirstVisibleCharacterPositionLeft)
			{
				++newFirstVisibleCharacter;
			}

			firstVisibleCharacter = newFirstVisibleCharacter;
		}

		Drawing::Size strWidth = textHelper.GetStringWidth(firstVisibleCharacter, caretPosition - firstVisibleCharacter);
		caretRect = Drawing::Rectangle(textRect.GetLeft() + strWidth.Width, textRect.GetTop(), 1, textRect.GetHeight());

		ResetCaretBlink();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void TextBox::OnMouseDown(const MouseMessage &mouse)
	{
		Control::OnMouseDown(mouse);

		Drawing::Size strWidth = textHelper.GetStringWidth(0, firstVisibleCharacter);
		PlaceCaret(textHelper.GetClosestCharacterIndex(mouse.Location - absoluteLocation + Drawing::Point(strWidth.Width - 7, 0)) - 1);
	}
	//---------------------------------------------------------------------------
	bool TextBox::OnKeyPress(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyPress(keyboard))
		{
			if (keyboard.KeyCode != Key::Return)
			{
				if (keyboard.KeyCode == Key::Back)
				{
					if (caretPosition > 0 && textHelper.GetLength() > 0)
					{
						textHelper.Remove(caretPosition - 1, 1);
						realtext.erase(caretPosition - 1, 1);
						PlaceCaret(caretPosition - 1);

						OnTextChanged();
					}
				}
				else if (keyboard.IsAlphaNumeric())
				{
					realtext.insert(caretPosition, 1, keyboard.KeyChar);
					if (passwordChar == '\0')
					{
						textHelper.Insert(caretPosition, keyboard.KeyChar);
					}
					else
					{
						textHelper.Insert(caretPosition, '*');
					}
					PlaceCaret(++caretPosition);
					
					OnTextChanged();
				}
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
	bool TextBox::OnKeyDown(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyDown(keyboard))
		{
			switch (keyboard.KeyCode)
			{
				case Key::Delete:
					if (caretPosition < textHelper.GetLength())
					{
						textHelper.Remove(caretPosition, 1);
						realtext.erase(caretPosition, 1);
						PlaceCaret(caretPosition);
						
						OnTextChanged();
					}
					break;
				case Key::Left:
					PlaceCaret(caretPosition - 1);
					break;
				case Key::Right:
					PlaceCaret(caretPosition + 1);
					break;
				case Key::Home:
					PlaceCaret(0);
					break;
				case Key::End:
					PlaceCaret(textHelper.GetLength());
					break;
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
	void TextBox::OnTextChanged()
	{
		textChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void TextBox::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		renderer->SetRenderColor(backColor - Drawing::Color(0, 20, 20, 20));
		renderer->Fill(absoluteLocation, size);
		renderer->SetRenderColor(backColor);
		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top + 1, GetWidth() - 2, GetHeight() - 2);
		
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, textRect, textHelper.GetText().substr(firstVisibleCharacter));

		if (Application::Instance()->GetNow() > nextBlinkTime)
		{
			showCaret = !showCaret;
			nextBlinkTime = Application::Instance()->GetNow().Add(blinkTime);
		}

		if (isFocused && showCaret)
		{
			renderer->Fill(caretRect);
		}
	}
	//---------------------------------------------------------------------------
}