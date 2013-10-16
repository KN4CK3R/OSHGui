/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
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
		type = CONTROL_PANEL;

		SetSize(DefaultSize);
		
		ApplyTheme(Application::Instance()->GetTheme());

		isFocusable = false;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Panel::Intersect(const Drawing::PointF &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void Panel::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
	
		if (backColor.A > 0)
		{
			renderer->SetRenderColor(backColor - Drawing::Color(0, 100, 100, 100));
			renderer->Fill(absoluteLocation, size);
			renderer->SetRenderColor(backColor);
			renderer->FillGradient(absoluteLocation.Left + 1, absoluteLocation.Top + 1, GetWidth() - 2, GetHeight() - 2, backColor - Drawing::Color(90, 90, 90));
		}
	
		ContainerControl::Render(renderer);
	}
	//---------------------------------------------------------------------------
	void Panel::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(geometry);
		g.Clear();

		if (backColor.A > 0)
		{
			g.FillRectangle(GetBackColor() - Color(0, 100, 100, 100), GetBounds());
			auto color = GetBackColor() - Color(0, 90, 90, 90);
			g.FillRectangleGradient(ColorRectangle(GetBackColor(), GetBackColor(), color, color), RectangleF(GetLocation() + PointF(1, 1), GetSize() - SizeF(2, 2)));
		}
	}
	//---------------------------------------------------------------------------
}
