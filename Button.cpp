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
		SetForeColor(Drawing::Color(0xFFE5E0E4));
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
		textHelper.SetFont(font);
		if (autoSize)
		{
			Drawing::Size size = textHelper.GetSize();
			size.Inflate(12, 10);
			SetSize(size);
		}
		clientArea = bounds;

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Button::ProcessEvent(Event *event)
	{
		if (event == 0)
		{
			return Event::DontContinue;
		}

		if (!visible || !enabled)
		{
			return Event::Continue;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position)) //ClientArea
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
					
						clickEventHandler.Invoke(this, mouse);
						
						//mouseClickEventHandler.Invoke
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
				clickEventHandler.Invoke(this, (MouseEvent*)0);
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
		
		Drawing::Color tempColor = backColor;

		if (hasFocus || mouseOver)
		{
			tempColor += mouseOverFocusColor;
		}
		
		renderer->SetRenderColor(tempColor + Drawing::Color(0, 10, 10, 10));
		renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop(), bounds.GetWidth() - 2, bounds.GetHeight() - 1);
		renderer->Fill(bounds.GetLeft(), bounds.GetTop() + 1, bounds.GetWidth(), bounds.GetHeight() - 3);
		renderer->SetRenderColor(tempColor - Drawing::Color(0, 50, 50, 50));
		renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop() + bounds.GetHeight() - 2, bounds.GetWidth() - 2, 2);
		renderer->Fill(bounds.GetLeft() + bounds.GetWidth() - 1, bounds.GetTop() + 1, 1, bounds.GetHeight() - 2);

		renderer->SetRenderColor(tempColor);
		renderer->FillGradient(bounds.GetLeft() + 1, bounds.GetTop() + 2, bounds.GetWidth() - 2, bounds.GetHeight() - 4, backColor - Drawing::Color(0, 20, 20, 20));
		renderer->FillGradient(bounds.GetLeft() + 2, bounds.GetTop() + 1, bounds.GetWidth() - 4, bounds.GetHeight() - 2, backColor - Drawing::Color(0, 20, 20, 20));

		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, bounds.GetLeft() + 6, bounds.GetTop() + 5, bounds.GetWidth() - 12, bounds.GetHeight() - 10, textHelper.GetText());

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