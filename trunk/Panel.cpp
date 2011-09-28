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
		SetForeColor(Drawing::Color::Empty());
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Panel::ContainsPoint(const Drawing::Point &point) const
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void Panel::Invalidate()
	{
		clientArea = bounds;

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Panel::ProcessEvent(Event *event)
	{
		if (event == 0)
		{
			return Event::DontContinue;
		}

		if (!visible || !enabled)
		{
			return Event::Continue;
		}

		Drawing::Point mousePositionBackup;
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
		}
	
		if (ChildProcessEvent(event) == Event::DontContinue)
		{
			return Event::DontContinue;
		}

		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			mouse->Position = mousePositionBackup;

			if (Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight()).Contains(mouse->Position))
			{
				if (mouse->State == MouseEvent::LeftDown || mouse->State == MouseEvent::RightDown)
				{
					mouseDownEvent.Invoke(this, MouseEventArgs(mouse));
				}
				else if (mouse->State == MouseEvent::Move)
				{
					mouseMoveEvent.Invoke(this, MouseEventArgs(mouse));
				}
				else if (mouse->State == MouseEvent::LeftUp || mouse->State == MouseEvent::RightUp)
				{
					mouseUpEvent.Invoke(this, MouseEventArgs(mouse));
				}
				
				return Event::DontContinue;
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
	
		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor - Drawing::Color(0, 100, 100, 100));
			renderer->Fill(bounds);
			renderer->SetRenderColor(backColor);
			renderer->FillGradient(bounds.GetLeft() + 1, bounds.GetTop() + 1, bounds.GetWidth() - 2, bounds.GetHeight() - 2, backColor - Drawing::Color(90, 90, 90));
		}
	
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