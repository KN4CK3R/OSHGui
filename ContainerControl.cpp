#include "ContainerControl.h"
#include "RadioButton.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void ContainerControl::AddControl(Control *control)
	{
		if (control != NULL)
		{
			controls.push_back(control);
		}
	}
	//---------------------------------------------------------------------------
	void ContainerControl::ClearRadioButtonGroup(int group)
	{
		for (unsigned int i = 0; i < controls.size(); i++)
		{
			Control *control = controls.at(i);

			if (control->GetType() == CONTROL_RADIOBUTTON)
			{
				RadioButton *radioButton = (RadioButton*)control;

				if (radioButton->GetGroup() == group)
				{
					radioButton->SetChecked(false);
				}
			}
		}
	}
	//---------------------------------------------------------------------------
	Control* ContainerControl::FindControlAtPoint(const Drawing::Point &point)
	{
		for (unsigned int i = 0; i < controls.size(); i++)
		{
			Control *control = controls.at(i);

			if (control == NULL)
			{
				continue;
			}

			if (control->GetEnabled() && control->GetVisible() && control->ContainsPoint(point))
			{
				return control;
			}
		}

		return NULL;
	}
	//---------------------------------------------------------------------------
	Control* ContainerControl::FindControlByName(const Misc::UnicodeString &name)
	{
		for (unsigned int i = 0; i < controls.size(); i++)
		{
			Control *control = controls.at(i);

			if (control == NULL)
			{
				continue;
			}
			
			if (control->GetName() == name)
			{
				return control;
			}
		}

		return NULL;
	}
	//---------------------------------------------------------------------------
}