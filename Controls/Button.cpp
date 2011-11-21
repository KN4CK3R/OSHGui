#include "Button.hpp"
#include "..\Misc\Exceptions.hpp"

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
		SetText(L"Button");
		
		SetBackColor(Drawing::Color(0xFF4E4E4E));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	Button::~Button()
	{
	
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Button::CanHaveFocus() const
	{
		return isVisible && isEnabled;
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
	/*bool Button::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}

		if (!isVisible || !isEnabled)
		{
			return false;
		}
	
		if (event->Type == IEvent::Mouse)
		{
			MouseMessage *mouse = (MouseMessage*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position)) //ClientArea
			{
				if (mouse->State == MouseMessage::LeftDown)
				{
					pressed = true;
				
					if (!isFocused)
					{
						parent->RequestFocus(this);
					}

					MouseEventArgs args(mouse);
					mouseDownEvent.Invoke(this, args);

					return true;
				}
				else if (mouse->State == MouseMessage::RightDown)
				{
					MouseEventArgs args(mouse);
					mouseDownEvent.Invoke(this, args);

					return true;
				}
				else if (mouse->State == MouseMessage::Move)
				{
					MouseEventArgs args(mouse);
					mouseMoveEvent.Invoke(this, args);

					return true;
				}
				else if (mouse->State == MouseMessage::LeftUp || mouse->State == MouseMessage::RightUp)
				{
					if (pressed && isFocused)
					{
						pressed = false;
					
						clickEvent.Invoke(this);
						
						MouseEventArgs args(mouse);
						mouseClickEvent.Invoke(this, args);
					}
					
					MouseEventArgs args(mouse);
					mouseUpEvent.Invoke(this, args);
					
					return true;
				}
			}

			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == IEvent::Keyboard)
		{
			KeyboardMessage *keyboard = (KeyboardMessage*) event;
			
			static Key::Keys oldKeyCode = Key::None;
			if (keyboard->State == KeyboardMessage::KeyDown)
			{
				oldKeyCode = keyboard->KeyCode;
				KeyEventArgs args(keyboard);
				keyDownEvent.Invoke(this, args);
			}
			else if (keyboard->State == KeyboardMessage::Character)
			{
				if (keyboard->KeyCode == Key::Return)
				{
					clickEvent.Invoke(this);
				}
				else if (isFocused)
				{
					KeyPressEventArgs args(keyboard);
					keyPressEvent.Invoke(this, args);
				}
			}
			else if (keyboard->State == KeyboardMessage::KeyUp)
			{
				if (keyboard->KeyCode == Key::Space && oldKeyCode == keyboard->KeyCode)
				{
					clickEvent.Invoke(this);
				}
				
				KeyEventArgs args(keyboard);
				keyUpEvent.Invoke(this, args);
			}
			
			return true;
		}
		
		return false;
	}
	//---------------------------------------------------------------------------*/
	void Button::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		Drawing::Color tempColor = backColor;

		if ((isFocused || mouseOver) && !(isFocused && pressed))
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
			
			ChildRender(renderer);
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}