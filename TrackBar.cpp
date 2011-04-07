#include "TrackBar.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TrackBar::TrackBar(Panel *parentPanel)
	{
		type = CONTROL_TRACKBAR;
		
		ParentPanel = parentPanel;
		
		pressed = false;
		
		min = 1;
		max = 10;
		value = 1;
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
		sliderMiddle = (int)((value - min) * (float)bounds.GetWidth() / (max - min));
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
	}
	//---------------------------------------------------------------------------
	int TrackBar::ValueFromPosition(int position)
	{ 
		float valuePerPixel = (float)(max - min) / bounds.GetWidth();
		return (int)(0.5f + min + valuePerPixel * (position - bounds.GetLeft())) ; 
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
			if (mouse->State == MouseEvent::LeftDown)
			{
				if (sliderRect.Contains(mouse->Position))
				{
					pressed = true;
					//SetCapture( DXUTGetHWND() );

					dragX = mouse->Position.X;
					dragOffset = sliderMiddle - dragX;

					if (!hasFocus)
					{
						ParentPanel->RequestFocus(this);
					}

					return Event::None;
				}

				if (bounds.Contains(mouse->Position))
				{
					dragX = mouse->Position.X;
					dragOffset = 0;               
					pressed = true;
					
					if (!hasFocus)
					{
						ParentPanel->RequestFocus(this);
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
					//ReleaseCapture();
					
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
					SetValueInternal(ValueFromPosition(mouse->Position.X + bounds.GetLeft() + dragOffset));
				}
			}
			return Event::None;
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
		if (needsRepaint)
		{
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture()); //track
				texture.Add(renderer->CreateNewTexture()); //button
			}
			
			Drawing::ITexture *track = texture.Get(0);
			{
				track->Create(bounds.GetSize());
				track->BeginUpdate();
				track->Clear();
				
				Drawing::Color color(0xFFAFAEAA);
				
				int range = max - min;
				float space = bounds.GetWidth() / (float)range;
				
				for (int i = 0; i < range; i++)
				{
					track->Fill(x + (i * space), 6, 1, 5, color);
				}

				track->EndUpdate();
			}
			
			Drawing::ITexture *slider = texture.Get(1);
			{
				slider->Create(sliderRect.GetSize());
				slider->BeginUpdate();
				
				slider->Fill(Color(0xFFAFAEAA));

				slider->EndUpdate();
			}
		}
		
		renderer->SetRenderColor(foreColor);
		renderer->RenderTexture(texture.Get(0), bounds.GetPosition());
		renderer->RenderTexture(texture.Get(1), sliderRect.GetPosition());
	}
	//---------------------------------------------------------------------------
}