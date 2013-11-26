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
		type = ControlType::LinkLabel;
		
		cursor = Cursors::Get(Cursors::Hand);

		canRaiseEvents = true;

		ApplyTheme(Application::Instance()->GetTheme());
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void LinkLabel::PopulateGeometry()
	{
		using namespace Drawing;

		Label::PopulateGeometry();

		Graphics g(*geometry);
		g.FillRectangle(GetForeColor(), RectangleF(PointF(GetLeft(), GetHeight()), SizeF(GetWidth(), 1)));
	}
	//---------------------------------------------------------------------------
}
