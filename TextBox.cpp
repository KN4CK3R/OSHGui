#include "TextBox.h"

namespace OSHGui
{
	TextBox::TextBox(Panel *parentPanel)
	{
		type = CONTROL_TEXTBOX;
		
		ParentPanel = parentPanel;

		blinkTime = 0.4;
		lastBlink = 0;//Gui::GlobalTime->GetAbsoluteTime();
		firstVisibleCharacter = 0;
		caretPosition = 0;
	}
		
	TextBox::~TextBox()
	{
			
	}
	
	bool TextBox::CanHaveFocus()
	{
		return enabled && visible;
	}
	bool TextBox::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	void TextBox::UpdateRects()
	{
		if (bounds.GetHeight() > TEXTBOX_DEFAULT_HEIGHT)
		{
			bounds.SetHeight(TEXTBOX_DEFAULT_HEIGHT);
		}
	
		textRect = bounds;
		textRect.Offset(7, 7);
		textRect.Inflate(-7, -7);
	}
	
	void TextBox::SetText(LPCSTR text)
	{
		if (text != NULL)
		{
			buffer.SetText(text);
			firstVisibleCharacter = 0;
			PlaceCaret(buffer.GetLength());
		}
	}
	LPCSTR TextBox::GetText()
	{
		return buffer.GetBuffer();
	}
	bool TextBox::GetTextCopy(LPSTR dest, int count)
	{
		if (dest == NULL || count > buffer.GetLength())
		{
			return false;
		}
	
		//wcscpy(dest, count, buffer.GetBuffer());

		return true;
	}
	
	int TextBox::GetTextLength()
	{
		return buffer.GetLength();
	}
	
	void TextBox::ClearText()
	{
		buffer.Clear();
		firstVisibleCharacter = 0;
		PlaceCaret(0);
	}
	
	void TextBox::PlaceCaret(int position)
	{
		caretPosition = position;

		int firstPos = buffer.CaretToPosition(firstVisibleCharacter);
		int caretPos = buffer.CaretToPosition(position);
		
		if (caretPos < firstPos)
		{
			firstVisibleCharacter = position;
		}
		else if (caretPos > firstPos + textRect.GetWidth())
		{
			int newLeftPos = caretPos - textRect.GetWidth();
			int newFirstPos = buffer.PositionToCaret(newLeftPos);
		
			if (buffer.CaretToPosition(newFirstPos) < newLeftPos)
			{
				newFirstPos++;
			}

			firstVisibleCharacter = newFirstPos;
		}
	}	
	void TextBox::PasteFromClipboard()
	{		
		if (IsClipboardFormatAvailable(CF_UNICODETEXT))
		{
			if (OpenClipboard(NULL))
			{
				HANDLE clipboard = GetClipboardData(CF_UNICODETEXT);
				if (clipboard != NULL)
				{
					char *data = (char*)GlobalLock(clipboard);
					if (data != NULL)
					{
						if (buffer.InsertString(caretPosition, data))
						{
							PlaceCaret(caretPosition + lstrlenW(data));
						}
						
						GlobalUnlock(clipboard);
					}
				}
				CloseClipboard();
			}
		}
	}
	
		
	Event::NextEventTypes TextBox::ProcessEvent(Event *event)
	{
		/*if (event == NULL || !enabled)
		{
			return Event::Continue;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*) event;
			
			if (clickFunc != NULL)
			{
				(*clickFunc)(mouse);
				if (mouse->Handled)
				{
					return Event::None;
				}
			}
			
			if (mouse->Button == MouseEvent::Left)
			{
				for (int i = drawPosition; i < text.size(); i++)
				{
				
				}
				
				return Event::Invalidate;
			}
		}
		else if (mesage->Type == Event::Keyboard)
		{
			bool hasChanged = true;
		
			KeyboardEvent *keyboard = (KeyboardEvent*) event;
			
			if (keyPressFunc != NULL)
			{
				(*keyPressFunc)(keyboard);
				if (keyboard->Handled)
				{
					return Event::None;
				}
			}
			
			if (keyboard->IsAlphaNumeric)
			{
				text.append(keyboard->GetCharacter());
			}
			else
			{
				switch (keyboard->KeyCode)
				{
					case Key::Back:
						if (position > 0 && text.size() > 0)
						{
							text.remove(position - 1, 1);
						}
						break;
					case Key::Delete:
						if (position < text.size() - 1)
						{
							text.remove(position, 1);
						}
						break;
					case Key::V:
						if (!keyboard->Control)
						{
							break;
						}
					case Key::Insert:
						PasteFromClipboard();
						break;
					case Key::Left:
						if (position > 0)
						{
							position--;
							hasChanged = false;
						}
						break;
					case Key::Right:
						if (position < text.size() - 1)
						{
							position++;
							hasChanged = false;
						}
						break;
					default:
						hasChanged = false;
				}
			}
			
			if (hasChanged)
			{
				if (changedFunc != NULL)
				{
					(*changedFunc)();
				}
			
				return Event::Invalidate;
			}
		}
		else if (event->Type == Event::Paint)
		{
			PaintEvent *paint = (PaintEvent*) event;
			Graphics *graphic = paint->Graphic;
			Rectangle clip = paint->Clip.Offset(bounds);
			
			graphics->FillRectangle(clip);
			graphics->DrawString(clip.GetPosition(), text);
			
			clip.SetWidth(1);
			clip.SetHeight(clip.GetHeight() - 6);
			clip.Offset(0 cursorPosition, 3);
			graphics->FillRectangle(clip);
		}*/
		
		return Event::None;
	}
	
	void TextBox::Render(Drawing::IRenderer *renderer)
	{
		//OK
		if (needRepaint)
		{
			if (texture == NULL)
			{
				texture = renderer->CreateNewTexture();
			}
			
			Drawing::Size size = bounds.GetSize();
			
			texture->Create(size);
			texture->BeginUpdate();
			texture->Clear();

			texture->FillGradient(1, 1, size.Width - 2, size.Height - 2, 0xFF373634, 0xFF383735);

			texture->Fill(1, 0, size.Width - 2, 1, 0x60FFFFFF);
			texture->Fill(0, 1, 1, size.Height - 2, 0x60FFFFFF);
			texture->Fill(1, size.Height - 1, size.Width - 2, 1, 0x60FFFFFF);
			texture->Fill(size.Width - 1, 1, 1, size.Height - 2, 0x60FFFFFF);

			texture->Fill(1, 1, 1, 1, 0x60FFFFFF);
			texture->Fill(size.Width - 2, 1, 1, 1, 0x60FFFFFF);
			texture->Fill(1, size.Height - 2, 1, 1, 0x60FFFFFF);
			texture->Fill(size.Width - 2, size.Height - 2, 1, 1, 0x60FFFFFF);

			texture->EndUpdate();
		}
		
		renderer->SetRenderColor();
		renderer->RenderTexture(texture, bounds);
		renderer->SetRenderColor();
		renderer->RenderText(font, textRect, text);
		
		//Carret
		renderer->Fill();
	}
}