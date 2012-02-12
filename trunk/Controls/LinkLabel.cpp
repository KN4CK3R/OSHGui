/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "LinkLabel.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	LinkLabel::LinkLabel()
	{
		type = CONTROL_LINKLABEL;

		SetText("LinkLabel");
		
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
		
		renderer->SetRenderColor(foreColor);
		renderer->Fill(absoluteLocation.Left, absoluteLocation.Top + GetHeight() - 1, GetWidth(), 1);

		Label::Render(renderer);
	}
	//---------------------------------------------------------------------------
}