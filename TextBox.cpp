#include "TextBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TextBox::TextBox(Control *parent) : Control(parent), textHelper(font)
	{
		type = CONTROL_TEXTBOX;

		blinkTime = 0.4;
		lastBlink = 0;//Gui::GlobalTime->GetAbsoluteTime();
		firstVisibleCharacter = 0;
		caretPosition = 0;

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
	void TextBox::SetText(const Misc::UnicodeString &text)
	{
		textHelper.SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& TextBox::GetText()
	{
		return textHelper.GetText();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool TextBox::CanHaveFocus()
	{
		return enabled && visible;
	}
	//---------------------------------------------------------------------------
	bool TextBox::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void TextBox::Invalidate()
	{
		if (bounds.GetHeight() > TEXTBOX_DEFAULT_HEIGHT)
		{
			bounds.SetHeight(TEXTBOX_DEFAULT_HEIGHT);
		}
	
		clientArea = bounds;
		textRect = Drawing::Rectangle(clientArea.GetLeft() + 7, clientArea.GetTop() + 5, clientArea.GetWidth() - 14, clientArea.GetHeight() - 10);
		caretRect = Drawing::Rectangle(0, 0, 1, textRect.GetHeight());
	}
	//---------------------------------------------------------------------------
	void TextBox::ClearText()
	{
		textHelper.Clear();
		firstVisibleCharacter = 0;
		PlaceCaret(0);
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
		if (position >= textHelper.GetLength())
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
			firstVisibleCharacter = position;
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
	}
	//---------------------------------------------------------------------------
	void TextBox::PasteFromClipboard()
	{		
		if (IsClipboardFormatAvailable(CF_UNICODETEXT))
		{
			if (OpenClipboard(NULL))
			{
				HANDLE clipboard = GetClipboardData(CF_UNICODETEXT);
				if (clipboard != NULL)
				{
					Misc::UnicodeChar *data = (Misc::UnicodeChar*)GlobalLock(clipboard);
					if (data != NULL)
					{
						Misc::UnicodeString strData(data);

						textHelper.Insert(caretPosition, strData);
						PlaceCaret(caretPosition + strData.length());
						
						GlobalUnlock(clipboard);
					}
				}
				CloseClipboard();
			}
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes TextBox::ProcessEvent(Event *event)
	{
		if (event == NULL || !enabled || !visible)
		{
			return Event::Continue;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*) event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			if (Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight()).Contains(mouse->Position)) //ClientArea
			{			
				if (mouse->State == MouseEvent::LeftDown)
				{
					if (!hasFocus)
					{
						Parent->RequestFocus(this);
					}

					Drawing::Size strWidth = textHelper.GetStringWidth(0, firstVisibleCharacter);
					PlaceCaret(textHelper.GetClosestCharacterIndex(mouse->Position + Drawing::Point(strWidth.Width - 7, 0)/*textRect padding*/) - 1);

					clickEventHandler.Invoke(this, mouse);
				
					return Event::DontContinue;
				}
			}

			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == Event::Keyboard)
		{
			bool hasChanged = false;
		
			KeyboardEvent *keyboard = (KeyboardEvent*)event;
			
			keyPressEventHandler.Invoke(this, keyboard);

			if (keyboard->State == KeyboardEvent::Character && keyboard->IsAlphaNumeric())
			{
				textHelper.Insert(caretPosition, keyboard->KeyChar);
				PlaceCaret(++caretPosition);
				hasChanged = true;
			}
			else if (keyboard->State == KeyboardEvent::Down)
			{
				switch (keyboard->KeyCode)
				{
					case Key::Back:
						if ((caretPosition > 0 || firstVisibleCharacter > 0) && textHelper.GetLength() > 0)
						{
							textHelper.Remove(caretPosition - 1, 1);
							if (firstVisibleCharacter > 0)
							{
								--firstVisibleCharacter;
								PlaceCaret(caretPosition);
							}
							else
							{
								PlaceCaret(caretPosition - 1);
							}
							hasChanged = true;
						}
						break;
					case Key::Delete:
						if (caretPosition < textHelper.GetLength())
						{
							textHelper.Remove(caretPosition + firstVisibleCharacter, 1);
							PlaceCaret(caretPosition);
							hasChanged = true;
						}
						break;
					case Key::V:
						if (!keyboard->Control)
						{
							break;
						}
					case Key::Insert:
						PasteFromClipboard();
						hasChanged = true;
						break;
					case Key::Left:
						PlaceCaret(caretPosition - 1);
						break;
					case Key::Right:
						PlaceCaret(caretPosition + 1);
						break;
					case Key::End:
						break;
				}
			}
			
			if (hasChanged)
			{
				changeEventHandler.Invoke(this);
			
				return Event::DontContinue;
			}
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void TextBox::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
		
		renderer->SetRenderColor(backColor - Drawing::Color(0, 20, 20, 20));
		renderer->Fill(bounds);
		renderer->SetRenderColor(backColor);
		renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop() + 1, bounds.GetWidth() - 2, bounds.GetHeight() - 2);
		
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, textRect, textHelper.GetText().substr(firstVisibleCharacter));
		
		if (hasFocus)
		{
			static DWORD time = GetTickCount();
			static DWORD time2;
			if (time + 700 > GetTickCount())
			{
				renderer->Fill(caretRect);
				time2 = GetTickCount();
			}
			else
				if (time2 + 700 < GetTickCount())
					time = GetTickCount();
		}

		if (controls.size() > 0)
		{
			Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
			renderer->SetRenderRectangle(clientArea + renderRect.GetPosition());
			
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				controls.at(i)->Render(renderer);
			}
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}