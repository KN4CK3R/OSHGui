#include "LinkLabel.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	LinkLabel::LinkLabel(const Misc::AnsiString &name, const Drawing::Point &location, const Drawing::Size &size, const Misc::AnsiString &text)
		: Label(name, location, size, text)
	{
		type = CONTROL_LINKLABEL;

		SetText(text);
		
		cursor = Cursors::Get(Cursors::Hand);

		canRaiseEvents = true;
	}
	//---------------------------------------------------------------------------
	LinkLabel::~LinkLabel()
	{

	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool LinkLabel::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
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
	
		renderer->Fill(GetLeft(), GetBottom() - 1, GetWidth(), 1);

		Label::Render(renderer);
	}
	//---------------------------------------------------------------------------
}