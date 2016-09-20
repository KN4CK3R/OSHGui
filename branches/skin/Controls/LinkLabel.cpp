/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
		type_ = ControlType::LinkLabel;
		
		cursor_ = Cursors::Get(Cursors::Hand);

		canRaiseEvents_ = true;

		ApplyStyle(Application::Instance().GetStyle());
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool LinkLabel::Intersect(const Drawing::PointI &point) const
	{
		return Control::Intersect(point);
	}
	//---------------------------------------------------------------------------
	void LinkLabel::PopulateGeometry(Skins::Base &skin)
	{
		skin.DrawLinkLabel(Drawing::Graphics(*geometry_), this);
	}
	//---------------------------------------------------------------------------
}
