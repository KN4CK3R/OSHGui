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

		SetLocation(Drawing::Point(9, 9));
		SetSize(Drawing::Size(92, 24));
		
		SetBackColor(Drawing::Color(0xFFF0F0F0));
		SetForeColor(Drawing::Color(0xFFD1CFCD));
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
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture());
			}
			
			Drawing::Size size = bounds.GetSize();

			Drawing::ITexture *main = texture.Get(0);
			
			main->Create(size);
			main->BeginUpdate();
			main->Clear();

			main->FillGradient(1, 1, size.Width - 2, size.Height - 2, Drawing::Color(0xFF635F5B), Drawing::Color(0xFF4E4D4A));

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

			needRepaint = false;
		}
		
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture.Get(0), bounds.GetPosition());
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, bounds, text);
	}
	//---------------------------------------------------------------------------
}