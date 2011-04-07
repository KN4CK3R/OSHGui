#include "Panel.h"
#include "RadioButton.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Panel::Panel(Panel *parentPanel)
	{
		type = CONTROL_PANEL;
		
		ParentPanel = parentPanel;
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
			Controls.Add(control);
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
		::ReleaseCapture();
	}
	//---------------------------------------------------------------------------
	void Panel::RequestFocus(Control *control)
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
	void Panel::ClearFocus()
	{
		if (focusControl)
		{
			focusControl->OnFocusOut();
			focusControl = NULL;
		}

		ReleaseCapture();
	}
	//---------------------------------------------------------------------------
	void Panel::ClearRadioButtonGroup(int group)
	{
		for (int i = 0; i < Controls.GetSize(); i++)
		{
			Control *control = Controls.Get(i);

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
		for (int i = 0; i < Controls.GetSize(); i++)
		{
			Control *control = Controls.Get(i);

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
	void Panel::OnMouseMove(const Drawing::Point &point)
	{
		Control *control = FindControlAtPoint(point);
		
		if (control == mouseOverControl)
		{
			return;
		}

		if (mouseOverControl)
		{
			mouseOverControl->OnMouseLeave();
		}

		mouseOverControl = control;
		if (control != NULL)
		{
			mouseOverControl->OnMouseEnter();
		}
	}
	//---------------------------------------------------------------------------
	void Panel::OnMouseUp(const Drawing::Point &point)
	{
		mouseOverControl = NULL;
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
			
			Control *control = FindControlAtPoint(mouse->Position);
			if (control != NULL)
			{
				if (control->ProcessEvent(event) == Event::None)
				{
					return Event::None;
				}
			}
			
			if (mouse->State == MouseEvent::LeftDown && focusControl != NULL)
			{
				focusControl->OnFocusOut();
				focusControl = NULL;
			}
			else if (mouse->State == MouseEvent::Move)
			{
				OnMouseMove(mouse->Position);
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
		//OK
		if (needsRepaint)
		{
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture());
			}
	
			Drawing::Size size = bounds.GetSize();
			
			Drawing::ITexture *main = texture.Get(0);
			
			main->Create(size);
			main->BeginUpdate();

			main->FillGradient(Color(0xFF3A3937), Color(0xFF4D4B49));

			main->EndUpdate();
		}
	
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture, bounds.GetPosition());
	
		Drawing::Rectangle rect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(bounds);
	
		for (int i = 0, len = Controls.GetSize(); i < len; i++)
		{
			Controls.Get(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(bounds);
	}
	//---------------------------------------------------------------------------
}