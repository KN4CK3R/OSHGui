#include "CheckBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	CheckBox::CheckBox(Panel *parentPanel)
	{
		type = CONTROL_CHECKBOX;
		
		ParentPanel = parentPanel;
		
		checked = false;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void CheckBox::SetChecked(bool checked)
	{
		if (this->checked != checked)
		{
			this->checked = checked;
			if (changeFunc != NULL)
			{
				(*changeFunc)(this);
			}
		}
	}
	//---------------------------------------------------------------------------
	bool CheckBox::GetChecked()
	{
		return checked;
	}
	//---------------------------------------------------------------------------
	void CheckBox::UpdateRects()
	{
		Button::UpdateRects();

		buttonRect = bounds;
		buttonRect.SetWidth(buttonRect.GetHeight());

		textRect = bounds;
		int offset = (int)(1.25f * buttonRect.GetWidth());
		textRect.Offset(offset, 2);
		textRect.Inflate(-offset, 0);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes CheckBox::ProcessEvent(Event *event)
	{
		if (event == NULL || !visible || !enabled)
		{
			return Event::None;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			if (mouse->State == MouseEvent::LeftDown)
			{
				pressed = true;
			
				if (!hasFocus)
				{
					ParentPanel->RequestFocus(this);
				}
			}
			else if (mouse->State == MouseEvent::LeftUp)
			{
				if (pressed && hasFocus)
				{
					SetChecked(!GetChecked());
					
					pressed = false;
				}
			}
			return Event::None;
		}
		else if (event->Type == Event::Keyboard)
		{
			KeyboardEvent *keyboard = (KeyboardEvent*)event;
			if (keyboard->KeyCode == Key::Space)
			{
				SetChecked(!GetChecked());
			}
			return Event::None;
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void CheckBox::Render(Drawing::IRenderer *renderer)
	{
		//OK
		if (needRepaint)
		{
			if (texture == NULL)
			{
				texture = renderer->CreateNewTexture();
			}

			texture->Create(Drawing::Size(15, 15));
			texture->BeginUpdate();
			texture->Clear();

			texture->Fill(1, 1, 13, 13, 0xFF444341);

			texture->Fill(0, 1, 1, 13, 0xBF8A7D71);
			texture->Fill(14, 1, 1, 13, 0xBF8A7D71);
			texture->Fill(1, 0, 13, 1, 0xBF8A7D71);
			texture->Fill(1, 14, 13, 1, 0xBF8A7D71);

			texture->Fill(1, 1, 1, 1, 0xBF8A7D71);
			texture->Fill(13, 1, 1, 1, 0xBF8A7D71);
			texture->Fill(1, 13, 1, 1, 0xBF8A7D71);
			texture->Fill(13, 13, 1, 1, 0xBF8A7D71);

			if (checked)
			{
				for (int i = 0; i < 4; i++)
				{
					texture->Fill(3 + i, 6 + i, 1, 3, 0xFFFFFFFF);
					texture->Fill(7 + i, 8 - i, 1, 3, 0xFFFFFFFF);
				}

				texture->Fill(11, 4, 1, 3, 0xFFFFFFFF);
			}

			texture->EndUpdate();
		}
		
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture, buttonRect);
		
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, textRect, text);
	}
	//---------------------------------------------------------------------------
}