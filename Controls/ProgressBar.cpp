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
	const Drawing::SizeI ProgressBar::DefaultSize(112, 24);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ProgressBar::ProgressBar()
		: min_(0),
		  max_(100),
		  value_(0)
	{
		type_ = ControlType::ProgressBar;

		SetSize(DefaultSize);
		
		ApplyStyle(Application::Instance().GetStyle());

		SetBarColor(Drawing::Color(0xFFf48a00));

		canRaiseEvents_ = false;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ProgressBar::SetMin(int min)
	{
		min_ = min;
		if (min_ > max_)
		{
			max_ = min_;
		}

		Adjust();
	}
	//---------------------------------------------------------------------------
	int ProgressBar::GetMin() const
	{
		return min_;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::SetMax(int max)
	{
		max_ = max;
		if (max_ < min_)
		{
			min_ = max_;
		}
		
		Adjust();
	}
	//---------------------------------------------------------------------------
	int ProgressBar::GetMax() const
	{
		return max_;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::SetValue(int value)
	{
		value_ = value;

		Adjust();

		Invalidate();
	}
	//---------------------------------------------------------------------------
	int ProgressBar::GetValue() const
	{
		return value_;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::SetBarColor(const Drawing::Color &color)
	{
		barColor_ = color;

		Invalidate();
	}
	//---------------------------------------------------------------------------
	const Drawing::Color& ProgressBar::GetBarColor() const
	{
		return barColor_;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ProgressBar::Intersect(const Drawing::PointI &point) const
	{
		return false;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::Adjust()
	{
		if (value_ < min_)
		{
			value_ = min_;
		}
		if (value_ > max_)
		{
			value_ = max_;
		}
	}
	//---------------------------------------------------------------------------
	void ProgressBar::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry_);

		for (int i = (int)(value_ / ((max_ - min_) / ((GetWidth() - 4) / 12.0f)) - 1); i >= 0; --i)
		{
			g.FillRectangle(barColor_, PointF(4 + i * 12, 4), SizeF(8, GetHeight() - 8));
		}
	}
	//---------------------------------------------------------------------------
}
