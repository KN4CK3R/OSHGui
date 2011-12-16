/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2011 KN4CK3R http://www.oldschoolhack.de
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
	const Drawing::Size ProgressBar::DefaultSize(110, 24);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ProgressBar::ProgressBar()
	{
		type = CONTROL_PROGRESSBAR;

		SetSize(DefaultSize);
		
		min = 0;
		max = 100;
		value = 0;

		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFF5A5857));
		SetBarColor(Drawing::Color(0xFF67AFF5));

		canRaiseEvents = false;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ProgressBar::SetMin(int min)
	{
		if (min < max)
		{
			this->min = min;
			Adjust();
		}
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		else
		{
			throw Misc::ArgumentException("min cannot be greater than max.", "min", __FILE__, __LINE__);
		}
		#endif
	}
	//---------------------------------------------------------------------------
	int ProgressBar::GetMin() const
	{
		return min;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::SetMax(int max)
	{
		if (max > min)
		{
			this->max = max;
			Adjust();
		}
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		else
		{
			throw Misc::ArgumentException("max cannot be lower than min.", "max", __FILE__, __LINE__);
		}
		#endif
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
	}
	//---------------------------------------------------------------------------
	int ProgressBar::GetValue() const
	{
		return value;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::SetBarColor(const Drawing::Color &color)
	{
		barColor = color;
	}
	//---------------------------------------------------------------------------
	const Drawing::Color& ProgressBar::GetBarColor() const
	{
		return barColor;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ProgressBar::Intersect(const Drawing::Point &point) const
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
	//Event-Handling
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
}