#include "RadioButton.hpp"
#include "..\Misc\Exceptions.hpp"
#include "ContainerControl.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	RadioButton::RadioButton()
		: CheckBox()
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
	void RadioButton::OnKeyUp(const KeyboardMessage &keyboard)
	{
		Control::OnKeyUp(keyboard);

		if (keyboard.KeyCode == Key::Space)
		{
			SetChecked(true);

			clickEvent.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
	//
	//---------------------------------------------------------------------------
	void RadioButton::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(checkBoxAbsolutePosition.Left, checkBoxAbsolutePosition.Top, DefaultCheckBoxSize, DefaultCheckBoxSize);

		Drawing::Color white = Drawing::Color::White();
		renderer->SetRenderColor(white);
		renderer->FillGradient(checkBoxAbsolutePosition.Left + 1, checkBoxAbsolutePosition.Top + 1, 15, 15, white - Drawing::Color(0, 137, 137, 137));
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(checkBoxAbsolutePosition.Left + 2, checkBoxAbsolutePosition.Top + 2, 13, 13, backColor + Drawing::Color(0, 55, 55, 55));
		
		if (checked)
		{
			renderer->Fill(checkBoxAbsolutePosition.Left + 6, checkBoxAbsolutePosition.Top + 4, 5, 9);
			renderer->Fill(checkBoxAbsolutePosition.Left + 4, checkBoxAbsolutePosition.Top + 6, 9, 5);
			renderer->Fill(checkBoxAbsolutePosition.Left + 5, checkBoxAbsolutePosition.Top + 5, 7, 7);
			
			renderer->SetRenderColor(white - Drawing::Color(0, 128, 128, 128));
			renderer->Fill(checkBoxAbsolutePosition.Left + 5, checkBoxAbsolutePosition.Top + 7, 7, 3);
			
			renderer->SetRenderColor(white);
			renderer->FillGradient(checkBoxAbsolutePosition.Left + 7, checkBoxAbsolutePosition.Top + 5, 3, 7, white - Drawing::Color(0, 137, 137, 137));
			renderer->FillGradient(checkBoxAbsolutePosition.Left + 6, checkBoxAbsolutePosition.Top + 6, 5, 5, white - Drawing::Color(0, 137, 137, 137));
		}

		label->Render(renderer);
	}
	//---------------------------------------------------------------------------
}