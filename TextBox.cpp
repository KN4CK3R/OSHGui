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
		firstVisibleCharacter = 5;
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
	
		clientArea = Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight());
		textRect = Drawing::Rectangle(7, 5, bounds.GetWidth() - 14, bounds.GetHeight() - 10);
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
			if (firstVisibleCharacter > 0)
			{
				--firstVisibleCharacter;
			}
			caretPosition = 0;
			caretRect = Drawing::Rectangle(-1, 0, 1, textRect.GetHeight());
		}
		else if (position == 0)
		{
			caretPosition = 0;
			caretRect = Drawing::Rectangle(-1, 0, 1, textRect.GetHeight());
		}
		else
		{
			static int lastWidth = -1;
			static int lastFirstVisibleCharacter = -1;

			Drawing::Point firstPos = textHelper.GetCharacterPosition(firstVisibleCharacter);
			Drawing::Point caretPos = textHelper.GetCharacterPosition(position + firstVisibleCharacter);

			if (caretPos.Left - firstPos.Left >= textRect.GetWidth())
			{
				++firstVisibleCharacter;
				caretPosition = --position;
			}
			else
			{
				caretPosition = position;
			}
			
			Drawing::Size strWidth = textHelper.GetStringWidth(firstVisibleCharacter, caretPosition);

			if (lastWidth == strWidth.Width && lastFirstVisibleCharacter == firstVisibleCharacter)
			{
				--caretPosition;
			}
			else
			{
				lastWidth = strWidth.Width;
			}
			lastFirstVisibleCharacter = firstVisibleCharacter;

			caretRect = Drawing::Rectangle(strWidth.Width - 1, 0, 1, textRect.GetHeight());
		}

		/*if (caretPos <= firstPos)
		{
			if (firstVisibleCharacter > 0)
			{
				--firstVisibleCharacter;
				++caretPosition;
				caretRect = Drawing::Rectangle(-2, 0, 1, textRect.GetHeight());
			}
		}
		else if (caretPos.Left - firstPos.Left >= textRect.GetWidth())
		{
			++firstVisibleCharacter;
			--caretPosition;
			caretPos = textHelper.GetCharacterPosition(caretPosition);
			caretRect = Drawing::Rectangle(caretPos.Left + 1, 0, 1, textRect.GetHeight());
		}
		else
		{
			caretPosition = position;
			caretRect = Drawing::Rectangle(caretPos.Left - firstPos.Left + 1, 0, 1, textRect.GetHeight());
		}*/
	}
	//---------------------------------------------------------------------------
	void TextBox::PasteFromClipboard()
	{		
		if (IsClipboardFormatAvailable(CF_TEXT))
		{
			if (OpenClipboard(NULL))
			{
				HANDLE clipboard = GetClipboardData(CF_TEXT);
				if (clipboard != NULL)
				{
					char *data = (char*)GlobalLock(clipboard);
					if (data != NULL)
					{
						//if (buffer.InsertString(caretPosition, data))
						{
							PlaceCaret(caretPosition + strlen(data));
						}
						
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

			if (clientArea.Contains(mouse->Position))
			{
				if (clickFunc != NULL)
				{
					(*clickFunc)(this, mouse);
					if (mouse->Handled)
					{
						return Event::DontContinue;
					}
				}
			
				if (mouse->State == MouseEvent::LeftDown)
				{
					if (!hasFocus)
					{
						Parent->RequestFocus(this);
					}

					PlaceCaret(textHelper.GetClosestCharacterIndex(mouse->Position) - 1);
				
					return Event::DontContinue;
				}
			}

			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == Event::Keyboard)
		{
			bool hasChanged = true;
		
			KeyboardEvent *keyboard = (KeyboardEvent*) event;
			
			if (keyPressFunc != NULL)
			{
				(*keyPressFunc)(this, keyboard);
				if (keyboard->Handled)
				{
					return Event::DontContinue;
				}
			}

			if (keyboard->State == KeyboardEvent::Character && keyboard->IsAlphaNumeric())
			{
				textHelper.Insert(caretPosition + firstVisibleCharacter, keyboard->KeyChar);
				PlaceCaret(++caretPosition);
			}
			else if (keyboard->State == KeyboardEvent::Down)
			{
				switch (keyboard->KeyCode)
				{
					case Key::Back:
						if ((caretPosition > 0 || firstVisibleCharacter > 0) && textHelper.GetLength() > 0)
						{
							textHelper.Remove(caretPosition + firstVisibleCharacter - 1, 1);
							PlaceCaret(caretPosition - 1);
						}
						break;
					case Key::Delete:
						if (caretPosition < textHelper.GetLength())
						{
							textHelper.Remove(caretPosition + firstVisibleCharacter, 1);
							PlaceCaret(caretPosition);
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
						hasChanged = false;
						break;
					case Key::Right:
						PlaceCaret(caretPosition + 1);
						hasChanged = false;
						break;
					default:
						hasChanged = false;
						break;
				}
			}
			
			if (hasChanged)
			{
				if (changeFunc != NULL)
				{
					(*changeFunc)(this);
				}
			
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

		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea + bounds.GetPosition() + renderRect.GetPosition());
			
		Drawing::Point position = bounds.GetPosition();
				
		renderer->SetRenderColor(backColor - Drawing::Color(0, 20, 20, 20));
		renderer->Fill(0, 0, clientArea.GetWidth(), clientArea.GetHeight());
		renderer->SetRenderColor(backColor);
		renderer->Fill(1, 1, clientArea.GetWidth() - 2, clientArea.GetHeight() - 2);
		
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, textRect, textHelper.GetText().substr(firstVisibleCharacter));
		
		if (hasFocus)
		{
			static DWORD time = GetTickCount();
			static DWORD time2;
			if (time + 700 > GetTickCount())
			{
				renderer->Fill(caretRect + textRect.GetPosition());
				time2 = GetTickCount();
			}
			else
				if (time2 + 700 < GetTickCount())
					time = GetTickCount();
		}

		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}