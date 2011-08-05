#include "Button.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Button::Button(Control *parent) : Label(parent)
	{
		type = CONTROL_BUTTON;
				
		pressed = false;

		SetAutoSize(false);
		SetSize(Drawing::Size(92, 24));
		
		SetBackColor(Drawing::Color(0xFF4E4E4E));
		SetForeColor(Drawing::Color::White());
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Button::CanHaveFocus()
	{
		return visible && enabled;
	}
	//---------------------------------------------------------------------------
	void Button::Invalidate()
	{
		if (autoSize)
		{
			Drawing::Size size = textHelper.GetSize();
			size.Width += 12;
			size.Height += 10;
			SetSize(size);
		}
		clientArea = Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight());
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Button::ProcessEvent(Event *event)
	{
		if (event == NULL || !visible || !enabled)
		{
			return Event::DontContinue;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			if (clientArea.Contains(mouse->Position))
			{
				if (mouse->State == MouseEvent::LeftDown)
				{
					pressed = true;
				
					if (!hasFocus)
					{
						Parent->RequestFocus(this);
					}
					return Event::DontContinue;
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
					return Event::DontContinue;
				}
			}

			mouse->Position = mousePositionBackup;
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
		if (!visible)
		{
			return;
		}

		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea + bounds.GetPosition() + renderRect.GetPosition());
			
		Drawing::Color tempColor = backColor;

		if (hasFocus || mouseOver)
		{
			tempColor += adjustColor;
		}
		
		renderer->SetRenderColor(tempColor + Drawing::Color(0, 10, 10, 10));
		renderer->Fill(1, 0, clientArea.GetWidth() - 2, clientArea.GetHeight() - 1);
		renderer->Fill(0, 1, bounds.GetWidth(), clientArea.GetHeight() - 3);
		renderer->SetRenderColor(tempColor - Drawing::Color(0, 50, 50, 50));
		renderer->Fill(1, clientArea.GetHeight() - 2, clientArea.GetWidth() - 2, 2);
		renderer->Fill(clientArea.GetWidth() - 1,  1, 1, clientArea.GetHeight() - 2);

		renderer->SetRenderColor(tempColor);
		renderer->FillGradient(1, 2, clientArea.GetWidth() - 2, clientArea.GetHeight() - 4, backColor - Drawing::Color(0, 20, 20, 20));
		renderer->FillGradient(2, 1, clientArea.GetWidth() - 4, clientArea.GetHeight() - 2, backColor - Drawing::Color(0, 20, 20, 20));

		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, 6, 5, clientArea.GetWidth() - 12, clientArea.GetHeight() - 10, textHelper.GetText());

		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}