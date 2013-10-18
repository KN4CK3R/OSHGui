/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "ProgressBar.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeF ProgressBar::DefaultSize(110, 24);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ProgressBar::ProgressBar()
		: min(0),
		  max(100),
		  value(0)
	{
		type = CONTROL_PROGRESSBAR;

		SetSize(DefaultSize);
		
		ApplyTheme(Application::Instance()->GetTheme());

		SetBarColor(Drawing::Color(0xFF67AFF5));

		canRaiseEvents = false;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ProgressBar::SetMin(int min)
	{
		this->min = min;
		if (min > max)
		{
			max = min;
		}

		Adjust();
	}
	//---------------------------------------------------------------------------
	int ProgressBar::GetMin() const
	{
		return min;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::SetMax(int max)
	{
		this->max = max;
		if (max < min)
		{
			min = max;
		}
		
		Adjust();
	}
	//---------------------------------------------------------------------------
	int ProgressBar::GetMax() const
	{
		return max;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::SetValue(int value)
	{
		this->value = value;
		Adjust();

		Invalidate();
	}
	//---------------------------------------------------------------------------
	int ProgressBar::GetValue() const
	{
		return value;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::SetBarColor(Drawing::Color color)
	{
		barColor = color;

		Invalidate();
	}
	//---------------------------------------------------------------------------
	Drawing::Color ProgressBar::GetBarColor() const
	{
		return barColor;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ProgressBar::Intersect(const Drawing::PointF &point) const
	{
		return false;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::Adjust()
	{
		if (value < min)
		{
			value = min;
		}
		if (value > max)
		{
			value = max;
		}
	}
	//---------------------------------------------------------------------------
	void ProgressBar::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top, GetWidth() - 2, GetHeight());
			renderer->Fill(absoluteLocation.Left, absoluteLocation.Top + 1, GetWidth(), GetHeight() - 2);
		}

		renderer->SetRenderColor(foreColor);
		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top, GetWidth() - 2, 1);
		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top + GetHeight() - 1, GetWidth() - 2, 1);
		renderer->Fill(absoluteLocation.Left, absoluteLocation.Top + 1, 1, GetHeight() - 2);
		renderer->Fill(absoluteLocation.Left + GetWidth() - 1, absoluteLocation.Top + 1, 1, GetHeight() - 2);

		renderer->SetRenderColor(barColor);
		for (int i = (int)(value / ((max - min) / ((GetWidth() - 8) / 12.0f)) - 1); i >= 0; --i)
		{
			renderer->Fill(absoluteLocation.Left + 4 + i * 12, absoluteLocation.Top + 4, 8, GetHeight() - 8);
		}
	}
	//---------------------------------------------------------------------------
	void ProgressBar::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(geometry);
		g.Clear();

		if (backColor.A != 0)
		{
			g.FillRectangle(GetBackColor(), PointF(1, 0), GetSize() - SizeF(2, 0));
			g.FillRectangle(GetBackColor(), PointF(0, 1), GetSize() - SizeF(0, 2));
		}

		g.FillRectangle(GetForeColor(), PointF(1, 0), SizeF(GetWidth() - 2, 1));
		g.FillRectangle(GetForeColor(), PointF(1, GetHeight() - 1), SizeF(GetWidth() - 2, 1));
		g.FillRectangle(GetForeColor(), PointF(0, 1), SizeF(1, GetHeight() - 2));
		g.FillRectangle(GetForeColor(), PointF(GetWidth() - 1, 1), SizeF(1, GetHeight() - 2));
		
		for (int i = (int)(value / ((max - min) / ((GetWidth() - 8) / 12.0f)) - 1); i >= 0; --i)
		{
			g.FillRectangle(barColor, PointF(4 + i * 12, 4), SizeF(8, GetHeight() - 8));
		}
	}
	//---------------------------------------------------------------------------
}
