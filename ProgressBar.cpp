#include "ProgressBar.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ProgressBar::ProgressBar(Control *parent) : Control(parent)
	{
		type = CONTROL_PROGRESSBAR;

		SetSize(Drawing::Size(110, 24));
		
		min = 0;
		max = 100;
		position = 0;

		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFF5A5857));
		SetBarColor(Drawing::Color(0xFF67AFF5));
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ProgressBar::SetMin(unsigned int min)
	{
		if (min < max)
		{
			this->min = min;
			Adjust();
		}
	}
	//---------------------------------------------------------------------------
	int ProgressBar::GetMin() const
	{
		return min;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::SetMax(unsigned int max)
	{
		if (max > min)
		{
			this->max = max;
			Adjust();
		}
	}
	//---------------------------------------------------------------------------
	int ProgressBar::GetMax() const
	{
		return max;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::SetPosition(unsigned int position)
	{
		this->position = position;
		Adjust();
	}
	//---------------------------------------------------------------------------
	int ProgressBar::GetPosition() const
	{
		return position;
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
	bool ProgressBar::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void ProgressBar::Adjust()
	{
		if (position < min)
		{
			position = min;
		}
		if (position > max)
		{
			position = max;
		}
	}
	//---------------------------------------------------------------------------
	void ProgressBar::Invalidate()
	{
		clientArea = bounds;

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void ProgressBar::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}

		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop(), bounds.GetWidth() - 2, bounds.GetHeight());
			renderer->Fill(bounds.GetLeft(), bounds.GetTop() + 1, bounds.GetWidth(), bounds.GetHeight() - 2);
		}

		renderer->SetRenderColor(foreColor);
		renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop(), bounds.GetWidth() - 2, 1);
		renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop() + bounds.GetHeight() - 1, bounds.GetWidth() - 2, 1);
		renderer->Fill(bounds.GetLeft(), bounds.GetTop() + 1, 1, bounds.GetHeight() - 2);
		renderer->Fill(bounds.GetLeft() + bounds.GetWidth() - 1, bounds.GetTop() + 1, 1, bounds.GetHeight() - 2);

		//TODO automatische höhe
		renderer->SetRenderColor(barColor);
		for (int i = (int)(position / ((max - min) / ((clientArea.GetWidth() - 8) / 12.0f)) - 1); i >= 0; --i)
		{
			renderer->Fill(bounds.GetLeft() + 4 + i * 12, bounds.GetTop() + 4, 8, bounds.GetHeight() - 8);
		}

		if (controls.size() > 0)
		{
			Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
			renderer->SetRenderRectangle(clientArea + renderRect.GetPosition());
			
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				controls.at(i)->Render(renderer);
			}
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}