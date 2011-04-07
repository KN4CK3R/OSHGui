#include "ProgressBar.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ProgressBar::ProgressBar(Panel *parentPanel)
	{
		type = CONTROL_PROGRESSBAR;
		
		ParentPanel = parentPanel;
		
		min = 0;
		max = 100;
		position = 0;
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
	//Event-Handling
	//---------------------------------------------------------------------------
	void ProgressBar::Render(Drawing::IRenderer *renderer)
	{
		if (needRepaint)
		{
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture());
			}

			Drawing::Size size = bounds.GetSize();
			
			Drawing::ITexture *main = texture.Get(0);

			main->Create(size);
			main->BeginUpdate();

			main->Fill(Color(0xFF2D2F2E));

			main->Fill(2, 2, position > 0 ? size.Width * ((float)(position - min) / (max - min)) - 4 : 0, size.Height - 4, Color(0xFF5F5F5F));

			main->EndUpdate();
		}

		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture.Get(0), bounds.GetPosition());
	}
	//---------------------------------------------------------------------------
}