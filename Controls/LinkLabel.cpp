#include "LinkLabel.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	LinkLabel::LinkLabel(Control *parent) : Label(parent)
	{
		type = CONTROL_LINKLABEL;

		SetText(L"LinkLabel");
		
		cursor = Cursors::Get(Cursors::Hand);
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
		if (!visible)
		{
			return;
		}
	
		Label::Render(renderer);
		
		renderer->Fill(bounds.GetLeft(), bounds.GetBottom() - 1, bounds.GetWidth(), 1);
	}
	//---------------------------------------------------------------------------
}