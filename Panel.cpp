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

		focusControl = NULL;
		mouseOverControl = NULL;
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
	void Panel::CaptureMouse(Control *control)
	{
		captureControl = control;
		//SetCapture(hwnd);
	}
	//---------------------------------------------------------------------------
	void Panel::ReleaseCapture()
	{
		captureControl = NULL;
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
		Drawing::Point client = PointToClient(point);

		for (unsigned int i = 0; i < Controls.size(); i++)
		{
			Control *control = Controls.at(i);

			if (control == NULL)
			{
				continue;
			}

			if (control->GetEnabled() && control->GetVisible() && control->ContainsPoint(client))
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

		if (event->Type == Event::Mouse)
		{
			Control *control = FindControlAtPoint(((MouseEvent*)event)->Position);
			if (control != mouseOverControl && mouseOverControl != NULL)
			{
				mouseOverControl->OnMouseLeave();
			}

			if (control != NULL)
			{
				mouseOverControl = control;
				mouseOverControl->OnMouseEnter();
			}
		}
	
		if (focusControl != NULL && focusControl->GetEnabled())
		{
			if (focusControl->ProcessEvent(event) == Event::None)
			{
				return Event::None;
			}
		}
		
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			
			if (mouseOverControl != NULL)
			{
				if (mouseOverControl->ProcessEvent(event) == Event::None)
				{
					return Event::None;
				}
			}
			
			if (mouse->State == MouseEvent::LeftDown && focusControl != NULL)
			{
				focusControl->OnFocusOut();
				focusControl = NULL;
			}
			return Event::Continue;
		}
		else if (event->Type == Event::System)
		{
			SystemEvent *system = (SystemEvent*)event;
			if (system->Type == SystemEvent::ActivateApp)
			{
				if (focusControl != NULL && focusControl->GetEnabled())
				{
					if (system->Value)
					{
						focusControl->OnFocusIn();
					}
					else
					{
						focusControl->OnFocusOut();
					}
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