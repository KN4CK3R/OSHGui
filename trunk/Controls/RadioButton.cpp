/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2011 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "RadioButton.hpp"
#include "../Misc/Exceptions.hpp"
#include "ContainerControl.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	RadioButton::RadioButton()
	{
		type = CONTROL_RADIOBUTTON;

		SetText("RadioButton");
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void RadioButton::SetChecked(bool checked)
	{
		if (this->checked != checked)
		{
			if (GetParent() != 0)
			{
				//uncheck other radiobuttons
				const std::list<Control*> &controls = ((ContainerControl*)GetParent())->GetControls();
				for (std::list<Control*>::const_iterator it = controls.begin(); it != controls.end(); ++it)
				{
					Control *control = *it;
					if (control->GetType() == CONTROL_RADIOBUTTON)
					{
						RadioButton *radio = (RadioButton*)control;
						radio->SetCheckedInternal(false);
					}
				}
			
				SetCheckedInternal(checked);
			}
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
	//Event-Handling
	//---------------------------------------------------------------------------
	void RadioButton::OnMouseClick(const MouseMessage &mouse)
	{
		Control::OnMouseClick(mouse);

		SetChecked(true);
	}
	//---------------------------------------------------------------------------
	bool RadioButton::OnKeyUp(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyUp(keyboard))
		{
			if (keyboard.KeyCode == Key::Space)
			{
				SetChecked(true);

				clickEvent.Invoke(this);
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void RadioButton::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(checkBoxAbsoluteLocation.Left, checkBoxAbsoluteLocation.Top, DefaultCheckBoxSize, DefaultCheckBoxSize);

		Drawing::Color white = Drawing::Color::White();
		renderer->SetRenderColor(white);
		renderer->FillGradient(checkBoxAbsoluteLocation.Left + 1, checkBoxAbsoluteLocation.Top + 1, 15, 15, white - Drawing::Color(0, 137, 137, 137));
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(checkBoxAbsoluteLocation.Left + 2, checkBoxAbsoluteLocation.Top + 2, 13, 13, backColor + Drawing::Color(0, 55, 55, 55));
		
		if (checked)
		{
			renderer->Fill(checkBoxAbsoluteLocation.Left + 6, checkBoxAbsoluteLocation.Top + 4, 5, 9);
			renderer->Fill(checkBoxAbsoluteLocation.Left + 4, checkBoxAbsoluteLocation.Top + 6, 9, 5);
			renderer->Fill(checkBoxAbsoluteLocation.Left + 5, checkBoxAbsoluteLocation.Top + 5, 7, 7);
			
			renderer->SetRenderColor(white - Drawing::Color(0, 128, 128, 128));
			renderer->Fill(checkBoxAbsoluteLocation.Left + 5, checkBoxAbsoluteLocation.Top + 7, 7, 3);
			
			renderer->SetRenderColor(white);
			renderer->FillGradient(checkBoxAbsoluteLocation.Left + 7, checkBoxAbsoluteLocation.Top + 5, 3, 7, white - Drawing::Color(0, 137, 137, 137));
			renderer->FillGradient(checkBoxAbsoluteLocation.Left + 6, checkBoxAbsoluteLocation.Top + 6, 5, 5, white - Drawing::Color(0, 137, 137, 137));
		}

		label->Render(renderer);
	}
	//---------------------------------------------------------------------------
}