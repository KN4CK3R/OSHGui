/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
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
		
		cursor = Cursors::Get(Cursors::Hand);

		canRaiseEvents = true;

		ApplyTheme(Application::Instance()->GetTheme());
	}
	//---------------------------------------------------------------------------
	LinkLabel::~LinkLabel()
	{

	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool LinkLabel::Intersect(const Drawing::PointF &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void LinkLabel::PopulateGeometry()
	{
		using namespace Drawing;

		Label::PopulateGeometry();

		Graphics g(geometry);
		g.FillRectangle(GetForeColor(), RectangleF(PointF(GetLeft(), GetHeight()), SizeF(GetWidth(), 1)));
	}
	//---------------------------------------------------------------------------
}
