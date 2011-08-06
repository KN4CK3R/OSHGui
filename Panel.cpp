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
		
		SetBackColor(Drawing::Color::Empty());
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
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Panel::ProcessEvent(Event *event)
	{
		if (event == NULL)
		{
			return Event::DontContinue;
		}
		
		if (!visible || !enabled)
		{
			return Event::Continue;
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
			Drawing::Point mousePositionBackup = mouse->Position;
			//mouse->Position = Panel::PointToClient(mouse->Position);
			
			//someone is capturing the mouse
			if (captureControl != NULL)
			{
				if (captureControl->ProcessEvent(mouse) == Event::DontContinue)
				{
					return Event::DontContinue;
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
					
					return Event::DontContinue;
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
	
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture.Get(0), bounds.GetPosition());
	
		Drawing::Rectangle rect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(bounds);
	
		for (unsigned int i = 0, len = controls.size(); i < len; i++)
		{
			controls.at(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(bounds);
	}
	//---------------------------------------------------------------------------
}