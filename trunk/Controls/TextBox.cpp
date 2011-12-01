#include "TextBox.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TextBox::TextBox() : Control(), textHelper(font)
	{
		type = CONTROL_TEXTBOX;

		SetBounds(6, 6, 100, font->GetSize() + 10);

		blinkTime = Misc::TimeSpan::FromMilliseconds(500);
		nextBlinkTime = Application::Instance()->GetNow();

		firstVisibleCharacter = 0;
		caretPosition = 0;
		
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
	void TextBox::SetText(const Misc::AnsiString &text)
	{
		textHelper.SetText(text);

		PlaceCaret(text.length());
		
		textChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& TextBox::GetText() const
	{
		return textHelper.GetText();
	}
	//---------------------------------------------------------------------------
	TextChangedEvent& TextBox::GetTextChangedEvent()
	{
		return textChangedEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool TextBox::CanHaveFocus() const
	{
		return isEnabled && isVisible;
	}
	//---------------------------------------------------------------------------
	bool TextBox::Intersect(const Drawing::Point &point) const
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void TextBox::Invalidate()
	{
		bounds.SetHeight(font->GetSize() + 10);
	
		clientArea = bounds;
		textRect = Drawing::Rectangle(clientArea.GetLeft() + 7, clientArea.GetTop() + 5, clientArea.GetWidth() - 14, clientArea.GetHeight() - 10);
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
	bool TextBox::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException("event", __FILE__, __LINE__);
		}

		if (!isVisible || !isEnabled)
		{
			return false;
		}
	
		if (event->Type == IEvent::Mouse)
		{
			MouseMessage *mouse = (MouseMessage*) event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			if (Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight()).Contains(mouse->Position)) //ClientArea
			{
				if (mouse->State == MouseMessage::LeftDown)
				{
					if (!isFocused)
					{
						parent->RequestFocus(this);
					}

					Drawing::Size strWidth = textHelper.GetStringWidth(0, firstVisibleCharacter);
					PlaceCaret(textHelper.GetClosestCharacterIndex(mouse->Position + Drawing::Point(strWidth.Width - 7, 0)/*textRect padding*/) - 1);

					clickEvent.Invoke(this);

					MouseEventArgs args(mouse);
					mouseDownEvent.Invoke(this, args);
				}
				else if (mouse->State == MouseMessage::Move)
				{
					MouseEventArgs args(mouse);
					mouseMoveEvent.Invoke(this, args);
				}
				else if (mouse->State == MouseMessage::LeftUp)
				{
					MouseEventArgs args(mouse);
					mouseClickEvent.Invoke(this, args);

					args = MouseEventArgs(mouse);
					mouseUpEvent.Invoke(this, args);
				}
				
				return true;
			}

			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == IEvent::Keyboard)
		{
			bool hasChanged = false;
		
			KeyboardMessage *keyboard = (KeyboardMessage*)event;

			if (keyboard->State == KeyboardMessage::Character)
			{
				if (keyboard->KeyCode != Key::Return)
				{
					KeyPressEventArgs args(keyboard);
					keyPressEvent.Invoke(this, args);
					if (!args.Handled)
					{
						if (keyboard->KeyCode == Key::Back)
						{
							if (caretPosition > 0 && textHelper.GetLength() > 0)
							{
								textHelper.Remove(caretPosition - 1, 1);
								PlaceCaret(caretPosition - 1);
								hasChanged = true;
							}
						}
						else if (isprint(keyboard->KeyChar))
						{
							textHelper.Insert(caretPosition, keyboard->KeyChar);
							PlaceCaret(++caretPosition);
							hasChanged = true;
						}
					}
				}
			}
			else if (keyboard->State == KeyboardMessage::KeyDown)
			{
				KeyEventArgs args(keyboard);
				keyDownEvent.Invoke(this, args);
				if (!args.Handled)
				{
					switch (keyboard->KeyCode)
					{
						case Key::Delete:
							if (caretPosition < textHelper.GetLength())
							{
								textHelper.Remove(caretPosition, 1);
								PlaceCaret(caretPosition);
								hasChanged = true;
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
			}
			else if (keyboard->State == KeyboardMessage::KeyUp)
			{
				KeyEventArgs args(keyboard);
				keyUpEvent.Invoke(this, args);
			}
			
			if (hasChanged)
			{
				textChangedEvent.Invoke(this);
			}
			
			return true;
		}
		
		return false;
	}
	//---------------------------------------------------------------------------
	void TextBox::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		renderer->SetRenderColor(backColor - Drawing::Color(0, 20, 20, 20));
		renderer->Fill(bounds);
		renderer->SetRenderColor(backColor);
		renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop() + 1, bounds.GetWidth() - 2, bounds.GetHeight() - 2);
		
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