/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "Panel.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeI Panel::DefaultSize(200, 200);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Panel::Panel()
	{
		type_ = ControlType::Panel;

		SetSize(DefaultSize);
		
		ApplyStyle(Application::Instance().GetStyle());

		isFocusable_ = false;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void Panel::PopulateGeometry(Skins::Base &skin)
	{
		skin.DrawPanel(Drawing::Graphics(*geometry_), this);
	}
	//---------------------------------------------------------------------------
}
