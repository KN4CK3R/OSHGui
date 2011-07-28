#include "LinkLabel.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	LinkLabel::LinkLabel(Panel *parentPanel)
	{
		type = CONTROL_LINKLABEL;

		ParentPanel = parentPanel;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes LinkLabel::ProcessEvent(Event *event)
	{
		if (event == NULL || !visible || !enabled)
		{
			return Event::None;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*) event;
			if (mouse->State == MouseEvent::LeftDown)
			{
				pressed = true;
			}
			if (mouse->State == MouseEvent::LeftUp)
			{
				if (pressed && hasFocus)
				{
					pressed = false;
					
					if (clickFunc != NULL)
					{
						(*clickFunc)(this, mouse);
					}
				}
			}
		}
		
		return Event::None;
	}
	//---------------------------------------------------------------------------
	void LinkLabel::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
	
		Label::Render(renderer);
		
		renderer->Fill(bounds.GetLeft(), bounds.GetBottom() + 1, bounds.GetWidth(), 1);
	}
	//---------------------------------------------------------------------------
}