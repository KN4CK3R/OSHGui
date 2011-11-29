#include "LinkLabel.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	LinkLabel::LinkLabel() : Label()
	{
		type = CONTROL_LINKLABEL;

		SetText(L"LinkLabel");
		
		cursor = Cursors::Get(Cursors::Hand);

		canRaiseEvents = true;
	}
	//---------------------------------------------------------------------------
	LinkLabel::~LinkLabel()
	{

	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void LinkLabel::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
	
		Label::Render(renderer);
		
		renderer->Fill(bounds.GetLeft(), bounds.GetBottom() - 1, bounds.GetWidth(), 1);
	}
	//---------------------------------------------------------------------------
}