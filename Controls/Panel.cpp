#include "Panel.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Panel::Panel()
		: ContainerControl()
	{
		type = CONTROL_PANEL;

		SetSize(Drawing::Size(200, 200));
				
		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color::Empty());
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Panel::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void Panel::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
	
		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor - Drawing::Color(0, 100, 100, 100));
			renderer->Fill(bounds);
			renderer->SetRenderColor(backColor);
			renderer->FillGradient(bounds.GetLeft() + 1, bounds.GetTop() + 1, bounds.GetWidth() - 2, bounds.GetHeight() - 2, backColor - Drawing::Color(90, 90, 90));
		}
	
		ContainerControl::Render(renderer);
	}
	//---------------------------------------------------------------------------
}