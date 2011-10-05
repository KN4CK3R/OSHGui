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
		SetSize(92, 24);
		
		SetBackColor(Drawing::Color(0xFF4E4E4E));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	Button::~Button()
	{
	
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	KeyDownEvent& Button::GetKeyDownEvent()
	{
		return keyDownEvent;
	}
	//---------------------------------------------------------------------------
	KeyPressEvent& Button::GetKeyPressEvent()
	{
		return keyPressEvent;
	}
	//---------------------------------------------------------------------------
	KeyUpEvent& Button::GetKeyUpEvent()
	{
		return keyUpEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Button::CanHaveFocus() const
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
	IEvent::NextEventTypes Button::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			return IEvent::DontContinue;
		}

		if (!visible || !enabled)
		{
			return IEvent::Continue;
		}
	
		if (event->Type == IEvent::Mouse)
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
						parent->RequestFocus(this);
					}

					mouseDownEvent.Invoke(this, MouseEventArgs(mouse));

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::RightDown)
				{
					mouseDownEvent.Invoke(this, MouseEventArgs(mouse));

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::Move)
				{
					mouseMoveEvent.Invoke(this, MouseEventArgs(mouse));

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftUp || mouse->State == MouseEvent::RightUp)
				{
					if (pressed && hasFocus)
					{
						pressed = false;
					
						clickEvent.Invoke(this);
						
						mouseClickEvent.Invoke(this, MouseEventArgs(mouse));
					}
					
					mouseUpEvent.Invoke(this, MouseEventArgs(mouse));
					
					return IEvent::DontContinue;
				}
			}

			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == IEvent::Keyboard)
		{
			KeyboardEvent *keyboard = (KeyboardEvent*) event;
			
			static Key::Keys oldKeyCode = Key::None;
			if (keyboard->State == KeyboardEvent::KeyDown)
			{
				oldKeyCode = keyboard->KeyCode;
				keyDownEvent.Invoke(this, KeyEventArgs(keyboard));
			}
			else if (keyboard->State == KeyboardEvent::Character)
			{
				if (keyboard->KeyCode == Key::Return)
				{
					clickEvent.Invoke(this);
				}
				else if (hasFocus)
				{
					keyPressEvent.Invoke(this, KeyPressEventArgs(keyboard));
				}
			}
			else if (keyboard->State == KeyboardEvent::KeyUp)
			{
				if (keyboard->KeyCode == Key::Space && oldKeyCode == keyboard->KeyCode)
				{
					clickEvent.Invoke(this);
				}
				
				keyUpEvent.Invoke(this, KeyEventArgs(keyboard));
			}
			
			return IEvent::DontContinue;
		}
		
		return IEvent::Continue;
	}
	//---------------------------------------------------------------------------
	void Button::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
		
		Drawing::Color tempColor = backColor;

		if ((hasFocus || mouseOver) && !(hasFocus && pressed))
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
				controls[i]->Render(renderer);
			}
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}