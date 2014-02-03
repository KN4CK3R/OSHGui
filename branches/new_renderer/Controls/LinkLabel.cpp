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

		ApplyTheme(Application::Instance().GetTheme());
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void LinkLabel::PopulateGeometry()
	{
		using namespace Drawing;

		Label::PopulateGeometry();

		Graphics g(*geometry_);
		g.FillRectangle(GetForeColor(), RectangleF(PointF(0, GetHeight()), SizeF(GetWidth(), 1)));
	}
	//---------------------------------------------------------------------------
}
