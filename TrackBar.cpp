#include "TrackBar.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TrackBar::TrackBar(Control *parent) : Control(parent)
	{
		type = CONTROL_TRACKBAR;
				
		pressed = false;
		
		min = 1;
		max = 10;
		value = 1;

		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFFA6A4A1));
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TrackBar::SetRange(int min, int max)
	{
		this->min = min;
		this->max = max;
		
		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetValue(int value)
	{
		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	int TrackBar::GetValue()
	{
		return value;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool TrackBar::CanHaveFocus()
	{
		return enabled && visible;
	}
	//---------------------------------------------------------------------------
	bool TrackBar::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void TrackBar::UpdateRects()
	{
		sliderMiddle = bounds.GetLeft() + ((value - min) * (float)bounds.GetWidth() / (max - min));
		sliderRect = Drawing::Rectangle(sliderMiddle - (TRACKBAR_SLIDER_WIDTH / 2), 0, TRACKBAR_SLIDER_WIDTH, TRACKBAR_SLIDER_HEIGHT);
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetValueInternal(int value)
	{
		value = _max(value, min);
		value = _min(value, max);
		
		if (this->value != value)
		{
			this->value = value;
			
			if (changeFunc != NULL)
			{
				(*changeFunc)(this);
			}
		}

		UpdateRects();
	}
	//---------------------------------------------------------------------------
	int TrackBar::ValueFromPosition(int position)
	{ 
		float valuePerPixel = (float)(max - min) / bounds.GetWidth();
		return (int)(0.5f + min + valuePerPixel * (position/* - bounds.GetLeft()*/)) ; 
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes TrackBar::ProcessEvent(Event *event)
	{	
		if (event == NULL || !visible || !enabled)
		{
			return Event::None;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			DrawingPoint mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			if (mouse->State == MouseEvent::LeftDown)
			{
				if (sliderRect.Contains(mouse->Position))
				{
					pressed = true;
					SetCapture(this);

					dragX = mouse->Position.X;
					dragOffset = sliderMiddle - dragX;

					if (!hasFocus)
					{
						Parent->RequestFocus(this);
					}

					return Event::None;
				}

				if (bounds.GetSize().Contains(mouse->Position))
				{
					dragX = mouse->Position.X;
					dragOffset = 0;               
					pressed = true;
					
					if (!hasFocus)
					{
						Parent->RequestFocus(this);
					}

					if (mouse->Position.X > sliderMiddle + bounds.GetLeft())
					{
						SetValueInternal(value + 1);
					}
					else if (mouse->Position.X < sliderMiddle + bounds.GetLeft())
					{
						SetValueInternal(value - 1);
					}
					return Event::None;
				}
			}
			else if (mouse->State == MouseEvent::LeftUp)
			{
				if (pressed)
				{
					pressed = false;
					ReleaseCapture();
					
					if (changeFunc != NULL)
					{
						(*changeFunc)(this);
					}

					return Event::None;
				}
			}
			else if (mouse->State == MouseEvent::Move)
			{
				if (pressed)
				{
					SetValueInternal(ValueFromPosition(mouse->Position.X/* - bounds.GetLeft() * 4 + dragOffset*/));
					return Event::None;
				}
			}
			
			//restore PointToClient (alternatively call PointToScreen)
			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == Event::Keyboard)
		{
			KeyboardEvent *keyboard = (KeyboardEvent*)event;
			switch (keyboard->KeyCode)
            {
                case Key::Home:
                    SetValueInternal(min);
                    return Event::None;
                case Key::End:
                    SetValueInternal(max);
                    return Event::None;
                case Key::Left:
                case Key::Down:
                    SetValueInternal(value - 1);
                    return Event::None;
                case Key::Right:
                case Key::Up:
                    SetValueInternal(value + 1);
                    return Event::None;
                case Key::PageDown:
                    SetValueInternal(value - _max(10, (max - min) / 10));
                    return Event::None;
                case Key::PageUp:
                    SetValueInternal(value + _max(10, (max - min) / 10));
                    return Event::None;
            }
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void TrackBar::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}

		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(bounds);
		}

		Drawing::Point position = bounds.GetPosition();

		renderer->SetRenderColor(hasFocus || mouseOver ? foreColor + Drawing::Color(0, 43, 43, 43) : foreColor);

		int range = max - min;
		float space = bounds.GetWidth() / (float)range;
		if (space < 5.0f)
		{
			space = 5.0f;
			range = bounds.GetWidth() / space;
		}
				
		for (int i = 0; i < range; i++)
		{
			renderer->Fill(position.Left + (i * space), position.Top + 6, 1, 5);
		}

		renderer->SetRenderColor(foreColor);
		renderer->Fill(sliderRect.OffsetEx(0, bounds.GetTop()));
	}
	//---------------------------------------------------------------------------
}