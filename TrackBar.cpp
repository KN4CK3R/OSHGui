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
	void TrackBar::Invalidate()
	{
		clientArea = Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight());

		sliderMiddle = (value - min) * (float)(clientArea.GetWidth() - TRACKBAR_SLIDER_WIDTH * 2) / (max - min) + TRACKBAR_SLIDER_WIDTH / 2;
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

		Invalidate();
	}
	//---------------------------------------------------------------------------
	int TrackBar::ValueFromPosition(int position)
	{ 
		float valuePerPixel = (float)(max - min) / (clientArea.GetWidth() - TRACKBAR_SLIDER_WIDTH);
		return (int)(0.5f + min + valuePerPixel * position) ; 
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes TrackBar::ProcessEvent(Event *event)
	{	
		if (event == NULL || !visible || !enabled)
		{
			return Event::DontContinue;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			if (mouse->State == MouseEvent::LeftDown)
			{
				if (sliderRect.Contains(mouse->Position))
				{
					pressed = true;
					//CaptureMouse(this);

					dragX = mouse->Position.X;
					dragOffset = sliderMiddle - dragX;

					if (!hasFocus)
					{
						Parent->RequestFocus(this);
					}

					return Event::DontContinue;
				}

				if (clientArea.Contains(mouse->Position))
				{
					dragX = mouse->Position.X;
					dragOffset = 0;               
					pressed = true;
					
					if (!hasFocus)
					{
						Parent->RequestFocus(this);
					}

					SetValueInternal(ValueFromPosition(mouse->Position.X));

					return Event::DontContinue;
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

					return Event::DontContinue;
				}
			}
			else if (mouse->State == MouseEvent::Move)
			{
				if (pressed)
				{
					SetValueInternal(ValueFromPosition(mouse->Position.X));
					return Event::DontContinue;
				}
			}
			
			//restore PointToClient (alternatively call PointToScreen)
			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == Event::Keyboard)
		{
			KeyboardEvent *keyboard = (KeyboardEvent*)event;
			if (keyboard->State == KeyboardEvent::Down)
			{
				switch (keyboard->KeyCode)
				{
					case Key::Home:
						SetValueInternal(min);
						return Event::DontContinue;
					case Key::End:
						SetValueInternal(max);
						return Event::DontContinue;
					case Key::Left:
					case Key::Down:
						SetValueInternal(value - 1);
						return Event::DontContinue;
					case Key::Right:
					case Key::Up:
						SetValueInternal(value + 1);
						return Event::DontContinue;
					case Key::PageDown:
						SetValueInternal(value - _max(10, (max - min) / 10));
						return Event::DontContinue;
					case Key::PageUp:
						SetValueInternal(value + _max(10, (max - min) / 10));
						return Event::DontContinue;
				}
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

		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea + bounds.GetPosition() + renderRect.GetPosition());

		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(clientArea);
		}

		Drawing::Point position = bounds.GetPosition();

		renderer->SetRenderColor(hasFocus || mouseOver ? foreColor + Drawing::Color(0, 43, 43, 43) : foreColor);

		int range = max - min;
		int halfWidth = TRACKBAR_SLIDER_WIDTH / 2;
		float space = (clientArea.GetWidth() - TRACKBAR_SLIDER_WIDTH) / (float)range;
		if (space < 5.0f)
		{
			space = 5.0f;
			range = (clientArea.GetWidth() - TRACKBAR_SLIDER_WIDTH) / space;
		}
				
		for (int i = 0; i < range; i++)
		{
			renderer->Fill(halfWidth + (i * space), 6, 1, 5);
		}

		renderer->SetRenderColor(foreColor);
		renderer->Fill(sliderRect);

		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}