/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "RadioButton.hpp"
#include "Label.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	RadioButton::RadioButton()
	{
		type_ = ControlType::RadioButton;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void RadioButton::SetChecked(bool checked)
	{
		if (checked_ != checked)
		{
			if (GetParent() != nullptr)
			{
				//uncheck other radiobuttons
				for (auto &control : GetParent()->GetControls())
				{
					if (control->GetType() == ControlType::RadioButton)
					{
						static_cast<RadioButton*>(control)->SetCheckedInternal(false);
					}
				}
			
				SetCheckedInternal(checked);
			}
		}
	}
	//---------------------------------------------------------------------------
	void RadioButton::SetCheckedInternal(bool checked)
	{
		if (checked_ != checked)
		{
			checked_ = checked;
			
			checkedChangedEvent_.Invoke(this);

			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	void RadioButton::PopulateGeometry(Skins::Base &skin)
	{
		skin.DrawRadioButton(Drawing::Graphics(*geometry_), this, Drawing::SizeF(DefaultCheckBoxSize, DefaultCheckBoxSize), checked_);
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
			if (keyboard.GetKeyCode() == Key::Space)
			{
				SetChecked(true);

				clickEvent_.Invoke(this);
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
	
}