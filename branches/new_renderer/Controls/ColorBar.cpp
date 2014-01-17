/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "ColorBar.hpp"
#include "../Misc/Exceptions.hpp"
#include "../Misc/Intersection.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeF ColorBar::DefaultSize(150, 45);
	const Drawing::SizeF ColorBar::DefaultBarSize(150, 10);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ColorBar::ColorBar()
		: barIndex(0),
		  color(Drawing::Color::Black())
	{
		type = ControlType::ColorBar;
	
		for (int i = 0; i < 3; ++i)
		{
			bars.push_back(Drawing::Color::Black());
			barSliderLocation.push_back(Drawing::PointF(0, i * 15 + 9));
			barSliderAbsoluteLocation.push_back(Drawing::PointF(0, 0));
			drag[i] = false;
		}
		
		SetSize(DefaultSize);
		
		ApplyTheme(Application::Instance().GetTheme());

		UpdateBars();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ColorBar::SetSize(const Drawing::SizeF &size)
	{
		if (this->size.Width != size.Width)
		{
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
	void ColorBar::UpdateColor()
	{
		float multi = 1.0f / (GetWidth() - 2);
		float newColor = multi * barSliderLocation[barIndex].Left;
		
		SetColor(Drawing::Color(barIndex == 0 ? newColor : color.GetRed(), barIndex == 1 ? newColor : color.GetGreen(), barIndex == 2 ? newColor : color.GetBlue()));
	}
	//---------------------------------------------------------------------------
	void ColorBar::UpdateBars()
	{
		float multi = GetWidth() - 2;
		for (int i = 0; i < 3; ++i)
		{
			bars[i].TopLeft = bars[i].BottomLeft = i == 0 ? Drawing::Color(0, color.GetGreen(), color.GetBlue()) : i == 1 ? Drawing::Color(color.GetRed(), 0, color.GetBlue()) : Drawing::Color(color.GetRed(), color.GetGreen(), 0);
			bars[i].TopRight = bars[i].BottomRight = color;
			
			barSliderLocation[i].Left = (int)((i == 0 ? color.GetRed() : i == 1 ? color.GetGreen() : color.GetBlue()) * multi);
			barSliderLocation[i].Top = i * 15 + 9;
			barSliderAbsoluteLocation[i] = absoluteLocation + barSliderLocation[i];
		}

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void ColorBar::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry);

		for (int i = 0; i < 3; ++i)
		{
			g.FillRectangleGradient(bars[i], RectangleF(PointF(0, i * 15), SizeF(GetWidth(), 8)));

			auto sliderPos = barSliderLocation[i] + PointF(1, 0);
			for (int j = 0; j < 3; ++j)
			{
				g.FillRectangle(GetForeColor(), RectangleF(PointF(sliderPos.X - j, sliderPos.Y + j), SizeF(1 + j * 2, 1)));
			}
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
			Drawing::PointF barLocation = Drawing::PointF(absoluteLocation.Left, absoluteLocation.Top + i * 15);
			if (Intersection::TestRectangle(barLocation, DefaultBarSize, mouse.Location))
			{
				barIndex = i;

				drag[barIndex] = true;

				OnGotMouseCapture();

				int localLocation = mouse.Location.Left - absoluteLocation.Left;
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

				UpdateColor();
			
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
			int localLocation = mouse.Location.Left - absoluteLocation.Left;
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
			
			UpdateColor();

			UpdateBars();
		}
	}
	//---------------------------------------------------------------------------
	bool ColorBar::OnKeyDown(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyDown(keyboard))
		{
			if (keyboard.GetKeyCode() == Key::Left || keyboard.GetKeyCode() == Key::Right)
			{
				barSliderLocation[barIndex].Left += keyboard.GetKeyCode() == Key::Left ? -1 : 1;
				
				if (barSliderLocation[barIndex].Left < 0)
				{
					barSliderLocation[barIndex].Left = 0;
				}
				if (barSliderLocation[barIndex].Left >= GetWidth() - 2)
				{
					barSliderLocation[barIndex].Left = GetWidth() - 2;
				}

				UpdateColor();

				UpdateBars();
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
}