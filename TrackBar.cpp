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
		if (bounds.GetHeight() != TRACKBAR_SLIDER_HEIGHT)
		{
			bounds.SetHeight(TRACKBAR_SLIDER_HEIGHT);
		}

		clientArea = bounds;

		sliderMiddle = (int)((value - min) * (float)(clientArea.GetWidth() - TRACKBAR_SLIDER_WIDTH) / (max - min) + TRACKBAR_SLIDER_WIDTH / 2);
		sliderRect = Drawing::Rectangle(clientArea.GetLeft() + sliderMiddle - (TRACKBAR_SLIDER_WIDTH / 2), clientArea.GetTop(), TRACKBAR_SLIDER_WIDTH, TRACKBAR_SLIDER_HEIGHT);

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetValueInternal(int value)
	{
		value = value < min ? min : value > max ? max : value;
		
		if (this->value != value)
		{
			this->value = value;
			
			changeEventHandler.Invoke(this);
		}

		Invalidate();
	}
	//---------------------------------------------------------------------------
	int TrackBar::ValueFromPosition(int position)
	{ 
		float valuePerPixel = (float)(max - min) / (clientArea.GetWidth() - TRACKBAR_SLIDER_WIDTH);
		return (int)(0.5f + min + valuePerPixel * position);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes TrackBar::ProcessEvent(Event *event)
	{	
		if (event == NULL)
		{
			return Event::DontContinue;
		}

		if (!visible || !enabled)
		{
			return Event::Continue;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			if (mouse->State == MouseEvent::LeftDown)
			{
				if (Drawing::Rectangle(sliderMiddle - (TRACKBAR_SLIDER_WIDTH / 2), 0, TRACKBAR_SLIDER_WIDTH, TRACKBAR_SLIDER_HEIGHT).Contains(mouse->Position)) //SliderRect
				{
					pressed = true;

					if (!hasFocus)
					{
						Parent->RequestFocus(this);
					}

					return Event::DontContinue;
				}

				if (Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight()).Contains(mouse->Position)) //ClientArea
				{
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
					
					changeEventHandler.Invoke(this);

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
						SetValueInternal(value - std::max(10, (max - min) / 10));
						return Event::DontContinue;
					case Key::PageUp:
						SetValueInternal(value + std::max(10, (max - min) / 10));
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

		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(bounds);
		}

		renderer->SetRenderColor(hasFocus || mouseOver ? foreColor + Drawing::Color(0, 43, 43, 43) : foreColor);

		int range = max - min;
		int halfWidth = TRACKBAR_SLIDER_WIDTH / 2;
		float space = (bounds.GetWidth() - TRACKBAR_SLIDER_WIDTH) / (float)range;
		if (space < 5.0f)
		{
			space = 5.0f;
			range = (int)((bounds.GetWidth() - TRACKBAR_SLIDER_WIDTH) / space);
		}
		
		for (int i = 0; i < range + 1; i++)
		{
			renderer->Fill((int)(bounds.GetLeft() + halfWidth + (i * space)), bounds.GetTop() + 6, 1, 5);
		}

		renderer->SetRenderColor(foreColor);
		renderer->Fill(sliderRect);

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