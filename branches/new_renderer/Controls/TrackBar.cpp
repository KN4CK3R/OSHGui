/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "TrackBar.hpp"
#include "../Misc/Exceptions.hpp"
#include "../Misc/Intersection.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeI TrackBar::SliderSize(8, 16);
	const Drawing::SizeI TrackBar::DefaultSize(110, TrackBar::SliderSize.Height + 2);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TrackBar::TrackBar()
		: drag(false),
		  minimum(1),
		  maximum(10),
		  tickFrequency(1),
		  sliderLocation(-SliderSize.Width / 2, 1)
	{
		type = ControlType::TrackBar;
	
		SetSize(DefaultSize);

		SetValueInternal(0);

		ApplyTheme(Application::Instance().GetTheme());
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TrackBar::SetSize(const Drawing::SizeI &size)
	{
		if (size.Height < SliderSize.Height + 2)
		{
			Control::SetSize(Drawing::SizeI(size.Width, SliderSize.Height + 2));
		}
		else
		{
			Control::SetSize(size);
		}

		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetMinimum(int minimum)
	{
		this->minimum = minimum;
		
		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	int TrackBar::GetMinimum() const
	{
		return minimum;
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetMaximum(int maximum)
	{
		this->maximum = maximum;
		
		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	int TrackBar::GetMaximum() const
	{
		return maximum;
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetTickFrequency(int tickFrequency)
	{
		this->tickFrequency = tickFrequency;
		
		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	int TrackBar::GetTickFrequency() const
	{
		return tickFrequency;
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetValue(int value)
	{
		SetValueInternal(value - minimum);
	}
	//---------------------------------------------------------------------------
	int TrackBar::GetValue() const
	{
		return value + minimum;
	}
	//---------------------------------------------------------------------------
	ValueChangedEvent& TrackBar::GetValueChangedEvent()
	{
		return valueChangedEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void TrackBar::SetValueInternal(int value_)
	{
		pixelsPerTick = (float)(GetWidth() - SliderSize.Width) / ((maximum - minimum) / tickFrequency);

		if (value_ < 0)
		{
			value_ = 0;
		}
		if (value_ > maximum - minimum)
		{
			value_ = maximum - minimum;
		}
		
		if (value != value_)
		{
			value = value_;
			
			valueChangedEvent.Invoke(this);

			int tick = value / tickFrequency;
			sliderLocation.Left = tick * pixelsPerTick;
			sliderAbsoluteLocation.Left = absoluteLocation.Left + sliderLocation.Left;

			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	void TrackBar::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();

		sliderAbsoluteLocation = absoluteLocation + sliderLocation;
	}
	//---------------------------------------------------------------------------
	void TrackBar::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry);

		if (backColor.GetAlpha() > 0)
		{
			g.FillRectangle(GetBackColor(), PointF(0, 0), GetSize());
		}

		auto color = isFocused || isInside ? GetForeColor() + Color::FromARGB(0, 43, 43, 43) : GetForeColor();

		auto tickCount = 1 + (maximum - minimum) / tickFrequency;
		for (int i = 0; i < tickCount; ++i)
		{
			auto x = SliderSize.Width / 2 + i * pixelsPerTick;
			g.FillRectangle(color, PointF(x, DefaultTickOffset), SizeF(1, 5));
		}

		g.FillRectangle(color, sliderLocation, SliderSize);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void TrackBar::OnMouseDown(const MouseMessage &mouse)
	{
		Control::OnMouseDown(mouse);

		if (Intersection::TestRectangle(sliderAbsoluteLocation, SliderSize, mouse.Location))
		{
			drag = true;
			OnGotMouseCapture();
		}
	}
	//---------------------------------------------------------------------------
	void TrackBar::OnMouseUp(const MouseMessage &mouse)
	{
		Control::OnMouseUp(mouse);

		drag = false;
		OnLostMouseCapture();
	}
	//---------------------------------------------------------------------------
	void TrackBar::OnMouseClick(const MouseMessage &mouse)
	{
		Control::OnMouseClick(mouse);

		if (!drag)
		{
			int tick = (mouse.Location.Left - absoluteLocation.Left) / pixelsPerTick;
			SetValueInternal(tick * tickFrequency);
		}
	}
	//---------------------------------------------------------------------------
	void TrackBar::OnMouseMove(const MouseMessage &mouse)
	{
		Control::OnMouseMove(mouse);

		if (drag)
		{
			int tick = (mouse.Location.Left - absoluteLocation.Left) / pixelsPerTick;
			SetValueInternal(tick * tickFrequency);
		}
	}
	//---------------------------------------------------------------------------
	void TrackBar::OnMouseScroll(const MouseMessage &mouse)
	{
		Control::OnMouseScroll(mouse);

		SetValueInternal(value + mouse.Delta);
	}
	//---------------------------------------------------------------------------
	bool TrackBar::OnKeyDown(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyDown(keyboard))
		{
			switch (keyboard.GetKeyCode())
			{
				case Key::Home:
					SetValueInternal(0);
					break;;
				case Key::End:
					SetValueInternal(maximum - minimum);
					break;
				case Key::Left:
				case Key::Down:
					SetValueInternal(value - 1);
					break;
				case Key::Right:
				case Key::Up:
					SetValueInternal(value + 1);
					break;
				case Key::PageDown:
					SetValueInternal(value - std::max(10, (maximum - minimum) / 10));
					break;
				case Key::PageUp:
					SetValueInternal(value + std::max(10, (maximum - minimum) / 10));
					break;
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
}