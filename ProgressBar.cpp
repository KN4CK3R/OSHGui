#include "ProgressBar.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ProgressBar::ProgressBar(Control *parent) : Control(parent)
	{
		type = CONTROL_PROGRESSBAR;
		
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
	int ProgressBar::GetMin()
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
	int ProgressBar::GetMax()
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
	int ProgressBar::GetPosition()
	{
		return position;
	}
	//---------------------------------------------------------------------------
	void ProgressBar::SetBarColor(const Drawing::Color &color)
	{
		barColor = color;
	}
	//---------------------------------------------------------------------------
	const Drawing::Color& ProgressBar::GetBarColor()
	{
		return barColor;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ProgressBar::CanHaveFocus()
	{
		return visible && enabled;
	}
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
		clientArea = Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight());
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

		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea + bounds.GetPosition() + renderRect.GetPosition());

		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(1, 0, clientArea.GetWidth() - 2, clientArea.GetHeight());
			renderer->Fill(0, 1, clientArea.GetWidth(), clientArea.GetHeight() - 2);
		}

		renderer->SetRenderColor(foreColor);
		renderer->Fill(1, 0, clientArea.GetWidth() - 2, 1);
		renderer->Fill(1, clientArea.GetHeight() - 1, clientArea.GetWidth() - 2, 1);
		renderer->Fill(0, 1, 1, clientArea.GetHeight() - 2);
		renderer->Fill(clientArea.GetWidth() - 1, 1, 1, clientArea.GetHeight() - 2);

		renderer->SetRenderColor(barColor);
		for (int i = position / ((max - min) / ((clientArea.GetWidth() - 8) / 12)) - 1; i >= 0; --i)
		{
			renderer->Fill(4 + i * 12, 4, 8, 16);
		}

		renderer->RenderText(font, 0, 30, Misc::Format(L"%i", position));

		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}