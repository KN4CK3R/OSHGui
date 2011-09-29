#include "RadioButton.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	RadioButton::RadioButton(Control *parent) : CheckBox(parent)
	{
		type = CONTROL_RADIOBUTTON;
		
		SetText(L"RadioButton");

		group = 0;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void RadioButton::SetChecked(bool checked)
	{
		if (this->checked != checked)
		{
			//uncheck other radiobuttons
			const std::vector<Control*> &controls = parent->GetControls();
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				Control *control = controls.at(i);
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
	IEvent::NextEventTypes RadioButton::ProcessEvent(IEvent *event)
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
				else if (mouse->State == MouseEvent::Move)
				{
					mouseMoveEvent.Invoke(this, MouseEventArgs(mouse));

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					if (pressed && hasFocus)
					{
						SetChecked(true);
						
						clickEvent.Invoke(this);
						
						mouseClickEvent.Invoke(this, MouseEventArgs(mouse));

						mouseUpEvent.Invoke(this, MouseEventArgs(mouse));
					
						pressed = false;
					}
					return IEvent::DontContinue;
				}
			}

			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == IEvent::Keyboard)
		{
			KeyboardEvent *keyboard = (KeyboardEvent*)event;
			if (keyboard->State == KeyboardEvent::KeyDown)
			{
				keyDownEvent.Invoke(this, KeyEventArgs(keyboard));
			}
			else if (keyboard->State == KeyboardEvent::Character)
			{
				keyPressEvent.Invoke(this, KeyPressEventArgs(keyboard));
			}
			else if (keyboard->State == KeyboardEvent::KeyUp)
			{
				if (keyboard->KeyCode == Key::Space)
				{
					SetChecked(!GetChecked());
					
					clickEvent.Invoke(this);
				}
				
				keyUpEvent.Invoke(this, KeyEventArgs(keyboard));
			}
			
			return IEvent::DontContinue;
		}
		
		return IEvent::Continue;
	}
	//---------------------------------------------------------------------------
	void RadioButton::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
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
		renderer->RenderText(font, textPosition.Left, textPosition.Top, bounds.GetWidth() - 20, bounds.GetHeight(), textHelper.GetText());

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