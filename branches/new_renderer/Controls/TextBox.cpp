/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
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
	const Drawing::SizeF TextBox::DefaultSize(100, 24);
	const Drawing::PointF TextBox::DefaultTextOffset(7, 5);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TextBox::TextBox()
		: textHelper(GetFont()),
		  blinkTime(Misc::TimeSpan::FromMilliseconds(500)),
		  firstVisibleCharacter(0),
		  caretPosition(0),
		  passwordChar('\0'),
		  showCaret(true)
	{
		type = ControlType::TextBox;
	
		ApplyTheme(Application::Instance()->GetTheme());

		SetSize(DefaultSize);
		
		cursor = Cursors::Get(Cursors::IBeam);
	}
	//---------------------------------------------------------------------------
	TextBox::~TextBox()
	{

	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TextBox::SetSize(const Drawing::SizeF &size)
	{
		Drawing::SizeF fixxed(size.Width, GetFont()->GetFontHeight() + DefaultTextOffset.Top * 2);

		Control::SetSize(fixxed);

		//textRect = Drawing::RectangleF(absoluteLocation.Left + DefaultTextOffset.Left, absoluteLocation.Top + DefaultTextOffset.Top, GetWidth() - DefaultTextOffset.Left * 2, GetHeight() - DefaultTextOffset.Top * 2);
		textRect = Drawing::RectangleF(DefaultTextOffset.Left, DefaultTextOffset.Top, GetWidth() - DefaultTextOffset.Left * 2, GetHeight() - DefaultTextOffset.Top * 2);

		firstVisibleCharacter = 0;
		PlaceCaret(textHelper.GetText().length());
	}
	//---------------------------------------------------------------------------
	void TextBox::SetFont(const Drawing::FontPtr &font)
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

		firstVisibleCharacter = 0;
		PlaceCaret(text.length());
		
		textChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& TextBox::GetText() const
	{
		return realtext;
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
	void TextBox::ShowCaret(bool showCaret)
	{
		this->showCaret = showCaret;
	}
	//---------------------------------------------------------------------------
	bool TextBox::Intersect(const Drawing::PointF &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void TextBox::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();
		
		//textRect = Drawing::RectangleF(absoluteLocation.Left + DefaultTextOffset.Left, absoluteLocation.Top + DefaultTextOffset.Top, GetWidth() - DefaultTextOffset.Left * 2, GetHeight() - DefaultTextOffset.Top * 2);
		textRect = Drawing::RectangleF(DefaultTextOffset.Left, DefaultTextOffset.Top, GetWidth() - DefaultTextOffset.Left * 2, GetHeight() - DefaultTextOffset.Top * 2);
		PlaceCaret(caretPosition);
	}
	//---------------------------------------------------------------------------
	void TextBox::ResetCaretBlink()
	{
		drawCaret = false;
		nextBlinkTime = Misc::DateTime();
	}
	//---------------------------------------------------------------------------
	void TextBox::PlaceCaret(int position)
	{
		if (position < 0)
		{
			position = 0;
		}
		caretPosition = position;

		Drawing::PointF caretPositionTrail;
		Drawing::PointF firstVisibleCharacterPosition = textHelper.GetCharacterPosition(firstVisibleCharacter);
		Drawing::PointF newCaretPosition = textHelper.GetCharacterPosition(position);

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
			int newFirstVisibleCharacter = textHelper.GetClosestCharacterIndex(Drawing::PointF(newFirstVisibleCharacterPositionLeft, 0));

			Drawing::PointF newFirstVisibleCharacterPosition = textHelper.GetCharacterPosition(newFirstVisibleCharacter);
			if (newFirstVisibleCharacterPosition.Left < newFirstVisibleCharacterPositionLeft)
			{
				++newFirstVisibleCharacter;
			}

			firstVisibleCharacter = newFirstVisibleCharacter;
		}

		Drawing::SizeF strWidth = textHelper.GetStringWidth(firstVisibleCharacter, caretPosition - firstVisibleCharacter);
		caretRect = Drawing::RectangleF(textRect.GetLeft() + strWidth.Width, textRect.GetTop(), 1, textRect.GetHeight());

		ResetCaretBlink();

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void TextBox::InjectTime(const Misc::DateTime &time)
	{
		if (time > nextBlinkTime)
		{
			drawCaret = !drawCaret;
			nextBlinkTime = time.Add(blinkTime);

			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	void TextBox::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(geometry);

		g.FillRectangle(GetBackColor() - Color(0, 20, 20, 20), PointF(0, 0), GetSize());
		g.FillRectangle(GetBackColor(), PointF(1, 1), GetSize() - SizeF(2, 2));

		g.DrawString(textHelper.GetText().substr(firstVisibleCharacter), GetFont(), GetForeColor(), textRect.GetLocation());

		if (showCaret)
		{
			if (isFocused && drawCaret)
			{
				g.FillRectangle(GetForeColor(), caretRect);
			}
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void TextBox::OnMouseDown(const MouseMessage &mouse)
	{
		Control::OnMouseDown(mouse);

		Drawing::SizeF strWidth = textHelper.GetStringWidth(0, firstVisibleCharacter);
		PlaceCaret(textHelper.GetClosestCharacterIndex(mouse.Location - absoluteLocation + Drawing::PointF(strWidth.Width - 7, 0)) - 1);
	}
	//---------------------------------------------------------------------------
	bool TextBox::OnKeyDown(const KeyboardMessage &keyboard)
	{
		Control::OnKeyDown(keyboard);

		switch (keyboard.GetKeyCode())
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
			case Key::Back:
				if (caretPosition > 0 && textHelper.GetLength() > 0)
				{
					textHelper.Remove(caretPosition - 1, 1);
					realtext.erase(caretPosition - 1, 1);
					PlaceCaret(caretPosition - 1);

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

		return false;
	}
	//---------------------------------------------------------------------------
	bool TextBox::OnKeyPress(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyPress(keyboard))
		{
			KeyEventArgs args(keyboard);
			if (keyboard.GetKeyCode() != Key::Return)
			{
				if (keyboard.IsAlphaNumeric())
				{
					realtext.insert(caretPosition, 1, keyboard.GetKeyChar());
					if (passwordChar == '\0')
					{
						textHelper.Insert(caretPosition, keyboard.GetKeyChar());
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
	void TextBox::OnTextChanged()
	{
		textChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
}
