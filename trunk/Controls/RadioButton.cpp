#include "RadioButton.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	RadioButton::RadioButton() : CheckBox("", Drawing::Point(), Drawing::Size(), "", false)
	{
		type = CONTROL_RADIOBUTTON;
		
		SetText("RadioButton");

		group = 0;

		canRaiseEvents = true;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void RadioButton::SetChecked(bool checked)
	{
		if (this->checked != checked)
		{
			//uncheck other radiobuttons
			const std::list<Control*> &controls = parent->GetControls();
			for (std::list<Control*>::const_iterator it = controls.begin(); it != controls.end(); ++it)
			{
				Control *control = *it;
				if (control->GetType() == CONTROL_RADIOBUTTON)
				{
					RadioButton *radio = (RadioButton*)control;
					if (radio->GetGroup() == group)
					{
						radio->SetCheckedInternal(false);
					}
				}
			}
			
			SetCheckedInternal(checked);
		}
	}
	//---------------------------------------------------------------------------
	void RadioButton::SetCheckedInternal(bool checked)
	{
		if (this->checked != checked)
		{
			this->checked = checked;
			
			checkedChangedEvent.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
	void RadioButton::SetGroup(int group)
	{
		this->group = group;
	}
	//---------------------------------------------------------------------------
	int RadioButton::GetGroup() const
	{
		return group;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void RadioButton::OnMouseClick(const MouseMessage &mouse)
	{
		SetChecked(true);

		CheckBox::OnMouseClick(mouse);
	}
	//---------------------------------------------------------------------------
	void RadioButton::OnKeyUp(const KeyboardMessage &keyboard)
	{
		SetChecked(true);

		CheckBox::OnKeyUp(keyboard);
	}
	//---------------------------------------------------------------------------
	/*bool RadioButton::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException("event", __FILE__, __LINE__);
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
				else if (mouse->State == MouseMessage::Move)
				{
					MouseEventArgs args(mouse);
					mouseMoveEvent.Invoke(this, args);

					return true;
				}
				else if (mouse->State == MouseMessage::LeftUp)
				{
					if (pressed && isFocused)
					{
						SetChecked(true);
						
						clickEvent.Invoke(this);
						
						MouseEventArgs args(mouse);
						mouseClickEvent.Invoke(this, args);

						args = MouseEventArgs(mouse);
						mouseUpEvent.Invoke(this, args);
					
						pressed = false;
					}
					return true;
				}
			}

			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == IEvent::Keyboard)
		{
			KeyboardMessage *keyboard = (KeyboardMessage*)event;
			if (keyboard->State == KeyboardMessage::KeyDown)
			{
				KeyEventArgs args(keyboard);
				keyDownEvent.Invoke(this, args);
			}
			else if (keyboard->State == KeyboardMessage::Character)
			{
				KeyPressEventArgs args(keyboard);
				keyPressEvent.Invoke(this, args);
			}
			else if (keyboard->State == KeyboardMessage::KeyUp)
			{
				if (keyboard->KeyCode == Key::Space)
				{
					SetChecked(!GetChecked());
					
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
	void RadioButton::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(checkBoxPosition.Left, checkBoxPosition.Top, 17, 17);

		Drawing::Color white = Drawing::Color::White();
		renderer->SetRenderColor(white);
		renderer->FillGradient(checkBoxPosition.Left + 1, checkBoxPosition.Top + 1, 15, 15, white - Drawing::Color(0, 137, 137, 137));
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(checkBoxPosition.Left + 2, checkBoxPosition.Top + 2, 13, 13, backColor + Drawing::Color(0, 55, 55, 55));
		
		if (checked)
		{
			renderer->Fill(checkBoxPosition.Left + 6, checkBoxPosition.Top + 4, 5, 9);
			renderer->Fill(checkBoxPosition.Left + 4, checkBoxPosition.Top + 6, 9, 5);
			renderer->Fill(checkBoxPosition.Left + 5, checkBoxPosition.Top + 5, 7, 7);
			
			renderer->SetRenderColor(white - Drawing::Color(0, 128, 128, 128));
			renderer->Fill(checkBoxPosition.Left + 5, checkBoxPosition.Top + 7, 7, 3);
			
			renderer->SetRenderColor(white);
			renderer->FillGradient(checkBoxPosition.Left + 7, checkBoxPosition.Top + 5, 3, 7, white - Drawing::Color(0, 137, 137, 137));
			renderer->FillGradient(checkBoxPosition.Left + 6, checkBoxPosition.Top + 6, 5, 5, white - Drawing::Color(0, 137, 137, 137));
		}

		renderer->SetRenderColor(foreColor);
		//renderer->RenderText(font, textPosition.Left, textPosition.Top, bounds.GetWidth() - 20, bounds.GetHeight(), textHelper.GetText());

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