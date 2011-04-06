#include "Button.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Button::Button(Panel *parentPanel)
	{
		type = CONTROL_BUTTON;
		
		ParentPanel = parentPanel;
		
		pressed = false;
		
		SetBackColor(Color(255, 0, 0, 0)); //todo
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Button::CanHaveFocus()
	{
		return visible && enabled;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Button::ProcessEvent(Event *event)
	{
		if (event == NULL || !visible || !enabled)
		{
			return Event::None;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*) event;
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
					pressed = false;
					
					if (clickFunc != NULL)
					{
						(*clickFunc)(this, mouse);
					}
				}
			}
			return Event::None;
		}
		else if (event->Type == Event::Keyboard)
		{
			KeyboardEvent *keyboard = (KeyboardEvent*) event;
			if (keyboard->KeyCode == Key::Return || keyboard->KeyCode == Key::Space)
			{
				if (clickFunc != NULL)
				{
					(*clickFunc)(this, NULL);
				}
			}
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void Button::Render(Drawing::IRenderer *renderer)
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

			texture->FillGradient(1, 1, size.Width - 2, size.Height - 2, 0xFF635F5B, 0xFF4E4D4A);

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
		
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture, bounds.GetPosition());
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, bounds, text);
	}
	//---------------------------------------------------------------------------
}