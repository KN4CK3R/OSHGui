#include "TextBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TextBox::TextBox(Control *parent) : Control(parent)
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
		if (text != NULL)
		{
			buffer.SetText(text);
			firstVisibleCharacter = 0;
			PlaceCaret(buffer.GetLength());
		}
	}
	//---------------------------------------------------------------------------
	Misc::UnicodeString& TextBox::GetText()
	{
		return buffer.GetBuffer();
	}
	//---------------------------------------------------------------------------
	int TextBox::GetTextLength()
	{
		return buffer.GetLength();
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
	//---------------------------------------------------------------------------
	void TextBox::ClearText()
	{
		buffer.Clear();
		firstVisibleCharacter = 0;
		PlaceCaret(0);
	}
	//---------------------------------------------------------------------------
	void TextBox::PlaceCaret(int position)
	{
		caretPosition = position;

		int firstPos = CharacterToPosition(firstVisibleCharacter);
		int caretPos = CharacterToPosition(position);
		
		if (caretPos < firstPos || caretPos > firstPos + textRect.GetWidth())
		{
			firstVisibleCharacter = position;
		}
	}
	//---------------------------------------------------------------------------
	int TextBox::CharacterToPosition(int charIndex)
	{
		const char *str = buffer.GetBuffer();
		
		int ret = 0;
		
		if (charIndex > 0) //todo
		{
			for (int i = 0; i < charIndex && str[i] != 0; i++)
			{
				ret += font->MeasureCharacter(str[i]);
			}
		}
		
		return ret;
	}
	//---------------------------------------------------------------------------
	int TextBox::PositionToCharacterIndex(int position)
	{
		const char *str = buffer.GetBuffer();
		
		int ret = 0;
		for (int i = 0; str[i] != 0; i++)
		{
			ret += font->MeasureCharacter(str[i]);
			if (ret >= position)
			{
				return i;
			}
		}
		
		return 0;
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
						if (buffer.InsertString(caretPosition, data))
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

			if (keyboard 
			
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
		}*/
		
		return Event::None;
	}
	//---------------------------------------------------------------------------
	void TextBox::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
	
		//OK
		/*if (needRepaint)
		{
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture());
			}
			
			Drawing::Size size = bounds.GetSize();
			
			Drawing::ITexture *main = texture.Get(0);
			
			main->Create(size);
			main->BeginUpdate();
			main->Clear();

			main->FillGradient(1, 1, size.Width - 2, size.Height - 2, 0xFF373634, 0xFF383735);
			
			Drawing::Color border(0x60FFFFFF);

			main->Fill(1, 0, size.Width - 2, 1, border);
			main->Fill(0, 1, 1, size.Height - 2, border);
			main->Fill(1, size.Height - 1, size.Width - 2, 1, border);
			main->Fill(size.Width - 1, 1, 1, size.Height - 2, border);

			main->Fill(1, 1, 1, 1, border);
			main->Fill(size.Width - 2, 1, 1, 1, border);
			main->Fill(1, size.Height - 2, 1, 1, border);
			main->Fill(size.Width - 2, size.Height - 2, 1, 1, border);

			main->EndUpdate();
		}*/
		
		Drawing::Point position = bounds.GetPosition();
				
		renderer->SetRenderColor(backColor - Drawing::Color(0, 20, 20, 20));
		renderer->Fill(position.Left, position.Top, bounds.GetWidth(), bounds.GetHeight());
		renderer->SetRenderColor(backColor);
		renderer->Fill(position.Left + 1, position.Top + 1, bounds.GetWidth() - 2, bounds.GetHeight() - 2);
		
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, textRect, buffer.GetBuffer() + firstVisibleCharacter);
		
		//Carret
		renderer->Fill(position.Left + 23, position.Top + 5, 1, 12);
	}
	//---------------------------------------------------------------------------
}