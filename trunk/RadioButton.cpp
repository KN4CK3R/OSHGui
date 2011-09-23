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
			const std::vector<Control*> &controls = Parent->GetControls();
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
	Event::NextEventTypes RadioButton::ProcessEvent(Event *event)
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
						SetChecked(true);
						
						clickEvent.Invoke(this);
						
						MouseEventArgs args(mouse->State, mouse->Position, mouse->Delta);
						mouseClickEvent.Invoke(this, args);
					
						pressed = false;
					}
					return Event::DontContinue;
				}
			}

			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == Event::Keyboard)
		{
			KeyboardEvent *keyboard = (KeyboardEvent*)event;
			if (keyboard->KeyCode == Key::Space)
			{
				SetChecked(!GetChecked());
				
				clickEvent.Invoke(this);
				
				return Event::DontContinue;
			}
		}
		
		return Event::Continue;
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