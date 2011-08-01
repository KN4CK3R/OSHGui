#include "Panel.h"
#include "RadioButton.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Panel::Panel(Control *parent) : Control(parent)
	{
		type = CONTROL_PANEL;
		
		SetBackColor(Drawing::Color(0xFFF0F0F0));
		SetForeColor(Drawing::Color(0xFFD1CFCD));
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Panel::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void Panel::AddControl(Control *control)
	{
		if (control != NULL)
		{
			//if (control->ParentForm != this)
			//{
			//	control->ParentForm = this;
			//}
			//Controls.Add(control);
			Controls.push_back(control);
		}
	}
	//---------------------------------------------------------------------------
	void Panel::ClearRadioButtonGroup(int group)
	{
		for (unsigned int i = 0; i < Controls.size(); i++)
		{
			Control *control = Controls.at(i);

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
	Control* Panel::FindControlAtPoint(const Drawing::Point &point)
	{
		for (unsigned int i = 0; i < Controls.size(); i++)
		{
			Control *control = Controls.at(i);

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
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Panel::ProcessEvent(Event *event)
	{
		if (event == NULL)
		{
			return Event::None;
		}
		
		if (!visible || !enabled)
		{
			return Event::Continue;
		}

		//someone is focused, so let him handle the event expect the mouse
		if (event->Type != Event::Mouse && focusControl != NULL && focusControl->GetVisible() && focusControl->GetEnabled())
		{
			if (focusControl->ProcessEvent(event) == Event::None)
			{
				return Event::None;
			}
		}		
			
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			//someone is capturing the mouse
			if (captureControl != NULL)
			{
				if (captureControl->ProcessEvent(mouse) == Event::None)
				{
					return Event::None;
				}
			}
			
			//find mouseOverControl
			Control *control = FindControlAtPoint(mousePositionBackup);
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
				if (focusControl->ProcessEvent(mouse) == Event::None)
				{
					return Event::None;
				}
			}
			
			//let mouseOverControl handle the mouse
			if (mouseOverControl != NULL)
			{
				if (mouseOverControl->ProcessEvent(event) == Event::None)
				{
					return Event::None;
				}
			}
			
			//restore PointToClient (alternatively call PointToScreen)
			mouse->Position = mousePositionBackup;
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
					
					return Event::None;
				}
			}
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void Panel::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
	
		//OK
		if (needRepaint)
		{
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture());
			}
	
			Drawing::Size size = bounds.GetSize();
			
			Drawing::ITexture *main = texture.Get(0);
			
			main->Create(size);
			main->BeginUpdate();

			main->FillGradient(Drawing::Color(0xFF3A3937), Drawing::Color(0xFF4D4B49));

			main->EndUpdate();
		}
	
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture.Get(0), bounds.GetPosition());
	
		Drawing::Rectangle rect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(bounds);
	
		for (unsigned int i = 0, len = Controls.size(); i < len; i++)
		{
			Controls.at(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(bounds);
	}
	//---------------------------------------------------------------------------
}