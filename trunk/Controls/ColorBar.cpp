#include "ColorBar.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::Size ColorBar::DefaultSize(150, 45);
	const Drawing::Size ColorBar::DefaultBarSize(150, 10);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ColorBar::ColorBar()
		: Control()
	{
		type = CONTROL_COLORBAR;
		
		barIndex = 0;
		
		for (int i = 0; i < 3; ++i)
		{
			bars.push_back(0);
			barSliderLocation.push_back(Drawing::Point(0, i * 15 + 9));
			barSliderAbsoluteLocation.push_back(Drawing::Point(0, 0));
			drag[i] = false;
		}
				
		SetSize(DefaultSize);
		
		color = Drawing::Color::Black();

		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFFE5E0E4));

		UpdateBars();
	}
	//---------------------------------------------------------------------------
	ColorBar::~ColorBar()
	{
	
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ColorBar::SetSize(const Drawing::Size &size)
	{
		if (this->size.Width != size.Width)
		{
			for (int i = 0; i < 3; ++i)
			{
				bars[i] = Application::Instance()->GetRenderer()->CreateNewTexture(size.Width, 10);
			}

			Control::SetSize(size);

			UpdateBars();
		}
		else
		{
			Control::SetSize(size);
		}
	}
	//---------------------------------------------------------------------------
	void ColorBar::SetColor(const Drawing::Color &color)
	{
		if (this->color != color)
		{
			this->color = color;

			UpdateBars();
		}

		Drawing::Color args = color;
		colorChangedEvent.Invoke(this, args);
	}
	//---------------------------------------------------------------------------
	const Drawing::Color& ColorBar::GetColor() const
	{
		return color;
	}
	//---------------------------------------------------------------------------
	ColorChangedEvent& ColorBar::GetColorChangedEvent()
	{
		return colorChangedEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ColorBar::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void ColorBar::CreateBarTexture(int index)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (index < 0 || index > 2)
		{
			throw Misc::ArgumentOutOfRangeException("index", __FILE__, __LINE__);
		}
		#endif
	
		std::shared_ptr<Drawing::ITexture> bar = bars[index];
		
		int width = GetWidth() - 2;	
		float multi = 255.0f / width;
		
		bar->BeginUpdate();
		bar->Fill(foreColor);
		for (int x = 0; x < width; ++x)
		{
			switch (index)
			{
				case 0:
					//bar->FillGradient(1, 1, width, 8, Drawing::Color(0, color.G, color.B), Drawing::Color(255, color.G, color.B), false);
					bar->Fill(x + 1, 1, 1, 8, Drawing::Color((unsigned int)(x * multi), this->color.G, this->color.B));
					break;
				case 1:
					//bar->FillGradient(1, 1, width, 8, Drawing::Color(color.R, 0, color.B), Drawing::Color(color.R, 255, color.B), false);
					bar->Fill(x + 1, 1, 1, 8, Drawing::Color(this->color.R, (unsigned int)(x * multi), this->color.B));
					break;
				case 2:
					//bar->FillGradient(1, 1, width, 8, Drawing::Color(color.R, color.G, 0), Drawing::Color(color.R, color.G, 255), false);
					bar->Fill(x + 1, 1, 1, 8, Drawing::Color(this->color.R, this->color.G, (unsigned int)(x * multi)));
					break;
			}
		}
		bar->EndUpdate();
	}
	//---------------------------------------------------------------------------
	void ColorBar::UpdateBars()
	{
		float multi = 255.0f / (GetWidth() - 2);			
		(barIndex == 0 ? color.R : barIndex == 1 ? color.G : color.B) = (unsigned char)(multi * barSliderLocation[barIndex].Left + 0.5f);

		multi = (GetWidth() - 2) / 255.0f;
		for (int i = 0; i < 3; ++i)
		{
			CreateBarTexture(i);
			
			barSliderLocation[i].Left = (int)((i == 0 ? color.R : i == 1 ? color.G : color.B) * multi + 0.5f);
			barSliderLocation[i].Top = i * 15 + 9;
			barSliderAbsoluteLocation[i] = absoluteLocation + barSliderLocation[i];
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void ColorBar::OnMouseDown(const MouseMessage &mouse)
	{
		Control::OnMouseDown(mouse);

		drag[0] = drag[1] = drag[2] = false;

		for (int i = 0; i < 3; ++i)
		{
			Drawing::Point barLocation = Drawing::Point(absoluteLocation.Left, absoluteLocation.Top + i * 15);
			if (Intersection::TestRectangle(barLocation, DefaultBarSize, mouse.Position))
			{
				barIndex = i;

				drag[barIndex] = true;

				OnGotMouseCapture();

				int localLocation = mouse.Position.Left - absoluteLocation.Left;
				if (localLocation < 0)
				{
					barSliderLocation[barIndex].Left = 0;
				}
				else if (localLocation > GetWidth() - 2)
				{
					barSliderLocation[barIndex].Left = GetWidth() - 2;
				}
				else
				{
					barSliderLocation[barIndex].Left = localLocation;
				}
			
				UpdateBars();

				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	void ColorBar::OnMouseUp(const MouseMessage &mouse)
	{
		Control::OnMouseUp(mouse);

		if (drag[barIndex])
		{
			drag[barIndex] = false;

			Drawing::Color colorArgs = color;
			colorChangedEvent.Invoke(this, colorArgs);

			OnLostMouseCapture();
		}
	}
	//---------------------------------------------------------------------------
	void ColorBar::OnMouseMove(const MouseMessage &mouse)
	{
		Control::OnMouseMove(mouse);

		if (drag[barIndex])
		{
			int localLocation = mouse.Position.Left - absoluteLocation.Left;
			if (localLocation < 0)
			{
				barSliderLocation[barIndex].Left = 0;
			}
			else if (localLocation > GetWidth() - 2)
			{
				barSliderLocation[barIndex].Left = GetWidth() - 2;
			}
			else
			{
				barSliderLocation[barIndex].Left = localLocation;
			}
			
			UpdateBars();

			Drawing::Color colorArgs = color;
			colorChangedEvent.Invoke(this, colorArgs);
		}
	}
	//---------------------------------------------------------------------------
	bool ColorBar::OnKeyDown(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyDown(keyboard))
		{
			if (keyboard.KeyCode == Key::Left || keyboard.KeyCode == Key::Right)
			{
				barSliderLocation[barIndex].Left += keyboard.KeyCode == Key::Left ? -1 : 1;
						
				if (barSliderLocation[barIndex].Left < 0)
				{
					barSliderLocation[barIndex].Left = 0;
				}
				if (barSliderLocation[barIndex].Left >= GetWidth() - 2)
				{
					barSliderLocation[barIndex].Left = GetWidth() - 2;
				}

				UpdateBars();

				Drawing::Color args = color;
				colorChangedEvent.Invoke(this, args);
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
	void ColorBar::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		for (int i = 0; i < 3; ++i)
		{
			renderer->SetRenderColor(Drawing::Color::White());
			renderer->RenderTexture(bars[i], absoluteLocation.Left, absoluteLocation.Top + i * 15, GetWidth(), 8);
			
			renderer->SetRenderColor(foreColor);
			Drawing::Point sliderPos = barSliderLocation[i].OffsetEx(absoluteLocation.Left + 1, absoluteLocation.Top);
			for (int j = 0; j < 3; ++j)
			{
				renderer->Fill(sliderPos.Left - j, sliderPos.Top + j, 1 + j * 2, 1);
			}
		}
	}
	//---------------------------------------------------------------------------
}