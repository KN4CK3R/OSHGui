#include "LinkLabel.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	LinkLabel::LinkLabel(Control *parent) : Label(parent)
	{
		type = CONTROL_LINKLABEL;

		SetText(L"LinkLabel");
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes LinkLabel::ProcessEvent(Event *event)
	{
		return Label::ProcessEvent(event);
	}
	//---------------------------------------------------------------------------
	void LinkLabel::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
	
		Label::Render(renderer);
		
		renderer->Fill(bounds.GetLeft(), bounds.GetBottom() - 1, bounds.GetWidth(), 1);
	}
	//---------------------------------------------------------------------------
}