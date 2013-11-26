/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
		type = ControlType::ProgressBar;

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
	void ProgressBar::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry);

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
