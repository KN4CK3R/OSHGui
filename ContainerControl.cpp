#include "ContainerControl.h"
#include "RadioButton.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ContainerControl::ContainerControl(Control *parent) : Control(parent)
	{
		focusControl = NULL;
		mouseOverControl = NULL;
		captureControl = NULL;
	}
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
	void ContainerControl::InvalidateChildren()
	{
		for (unsigned int i = 0; i < controls.size(); i++)
		{
			Control *control = controls.at(i);

			if (control == NULL)
			{
				continue;
			}

			control->Invalidate();
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
	void ContainerControl::RequestFocus(Control *control)
	{
		if (focusControl == control)
		{
			return;
		}

		if (!control->CanHaveFocus())
		{
			return;
		}

		if (focusControl != NULL)
		{
			focusControl->OnFocusOut();
		}

		control->OnFocusIn();
		focusControl = control;
	}
	//---------------------------------------------------------------------------
	void ContainerControl::ClearFocus()
	{
		if (focusControl)
		{
			focusControl->OnFocusOut();
			focusControl = NULL;
		}

		ReleaseCapture();
	}
	//---------------------------------------------------------------------------
	void ContainerControl::CaptureMouse(Control *control)
	{
		captureControl = control;
	}
	//---------------------------------------------------------------------------
	void ContainerControl::ReleaseCapture()
	{
		captureControl = NULL;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes ContainerControl::ContainerProcessEvent(Event *event)
	{
		if (event == NULL)
		{
			return Event::DontContinue;
		}
		
		//someone is focused, so let him handle the event expect the mouse
		if (event->Type != Event::Mouse && focusControl != NULL && focusControl->GetVisible() && focusControl->GetEnabled())
		{
			if (focusControl->ProcessEvent(event) == Event::DontContinue)
			{
				return Event::DontContinue;
			}
		}		
			
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			
			//someone is capturing the mouse
			if (captureControl != NULL)
			{
				if (captureControl->ProcessEvent(mouse) == Event::DontContinue)
				{
					return Event::DontContinue;
				}
			}
			
			//find mouseOverControl
			Control *control = FindControlAtPoint(mouse->Position);
			if (control != mouseOverControl && mouseOverControl != NULL)
			{
				mouseOverControl->OnMouseLeave();
			}

			if (control != NULL)
			{
				mouseOverControl = control;
				mouseOverControl->OnMouseEnter();
			}
			
			//someone is focused
			if (focusControl != NULL && focusControl->GetEnabled())
			{
				if (focusControl->ProcessEvent(mouse) == Event::DontContinue)
				{
					return Event::DontContinue;
				}
			}
			
			//let mouseOverControl handle the mouse
			if (mouseOverControl != NULL)
			{
				if (mouseOverControl->ProcessEvent(event) == Event::DontContinue)
				{
					return Event::DontContinue;
				}
			}
		}
		else if (event->Type == Event::System)
		{
			SystemEvent *system = (SystemEvent*)event;
			if (system->Type == SystemEvent::ActivateApp)
			{
				if (focusControl != NULL && focusControl->GetVisible() && focusControl->GetEnabled())
				{
					if (system->Value)
					{
						focusControl->OnFocusIn();
					}
					else
					{
						focusControl->OnFocusOut();
					}
					
					return Event::DontContinue;
				}
			}
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
}