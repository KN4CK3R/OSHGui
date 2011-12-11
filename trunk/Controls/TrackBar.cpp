#include "TrackBar.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::Size TrackBar::SliderSize(8, 16);
	const Drawing::Size TrackBar::DefaultSize(110, TrackBar::SliderSize.Height + 2);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TrackBar::TrackBar()
	{
		type = CONTROL_TRACKBAR;
		
		drag = false;
		
		minimum = 1;
		maximum = 10;
		tickFrequency = 1;

		sliderLocation = Drawing::Point(-SliderSize.Width / 2, 1);

		SetSize(DefaultSize);

		SetValueInternal(0);

		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFFA6A4A1));
	}
	//---------------------------------------------------------------------------
	TrackBar::~TrackBar()
	{

	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TrackBar::SetSize(const Drawing::Size &size)
	{
		if (size.Height < SliderSize.Height + 2)
		{
			Control::SetSize(Drawing::Size(size.Width, SliderSize.Height + 2));
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
	bool TrackBar::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetValueInternal(int value)
	{
		pixelsPerTick = (GetWidth() - SliderSize.Width) / ((maximum - minimum) / tickFrequency);

		if (value < 0)
		{
			value = 0;
		}
		if (value > maximum - minimum)
		{
			value = maximum - minimum;
		}
		
		if (this->value != value)
		{
			this->value = value;
			
			valueChangedEvent.Invoke(this);

			int tickPosition = value / tickFrequency;
			sliderLocation.Left = tickPosition * pixelsPerTick;
			sliderAbsoluteLocation.Left = absoluteLocation.Left + sliderLocation.Left;
		}
	}
	//---------------------------------------------------------------------------
	void TrackBar::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();

		sliderAbsoluteLocation = absoluteLocation + sliderLocation;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void TrackBar::OnMouseDown(const MouseMessage &mouse)
	{
		Control::OnMouseDown(mouse);

		if (Intersection::TestRectangle(sliderAbsoluteLocation, SliderSize, mouse.Position))
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

		float oldSliderValue = value;
		int tickPosition;
		if (!drag)
		{
			tickPosition = (mouse.Position.Left - absoluteLocation.Left) / pixelsPerTick;
			SetValueInternal(tickPosition * tickFrequency);
		}
	}
	//---------------------------------------------------------------------------
	void TrackBar::OnMouseMove(const MouseMessage &mouse)
	{
		Control::OnMouseMove(mouse);

		if (drag)
		{
			int tickPosition = (mouse.Position.Left - absoluteLocation.Left) / pixelsPerTick;
			SetValueInternal(tickPosition * tickFrequency);
		}
	}
	//---------------------------------------------------------------------------
	bool TrackBar::OnKeyDown(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyDown(keyboard))
		{
			switch (keyboard.KeyCode)
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
	void TrackBar::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(absoluteLocation, size);
		}

		renderer->SetRenderColor(isFocused || mouseOver ? foreColor + Drawing::Color(0, 43, 43, 43) : foreColor);

		int tickCount = 1 + (maximum - minimum) / tickFrequency;
		
		for (int i = 0; i < tickCount; ++i)
		{
			int x = absoluteLocation.Left + SliderSize.Width / 2 + i * pixelsPerTick;
            int y = absoluteLocation.Top + DefaultTickOffset;
			renderer->Fill(x, y, 1, 5);
		}

		renderer->SetRenderColor(foreColor);
		renderer->Fill(sliderAbsoluteLocation, SliderSize);
	}
	//---------------------------------------------------------------------------
}