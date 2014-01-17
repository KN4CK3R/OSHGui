/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "Panel.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeF Panel::DefaultSize(200, 200);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Panel::Panel()
	{
		type = ControlType::Panel;

		SetSize(DefaultSize);
		
		ApplyTheme(Application::Instance().GetTheme());

		isFocusable = false;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void Panel::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry);

		if (backColor.GetAlpha() > 0)
		{
			g.FillRectangle(GetBackColor() - Color::FromARGB(0, 100, 100, 100), GetBounds());
			auto color = GetBackColor() - Color::FromARGB(0, 90, 90, 90);
			g.FillRectangleGradient(ColorRectangle(GetBackColor(), GetBackColor(), color, color), RectangleF(GetLocation() + PointF(1, 1), GetSize() - SizeF(2, 2)));
		}
	}
	//---------------------------------------------------------------------------
}
