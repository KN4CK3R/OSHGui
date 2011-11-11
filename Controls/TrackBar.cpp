#include "TrackBar.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	const int TrackBar::trackbarSliderWidth = 8;
	const int TrackBar::trackbarSliderHeight = 16;
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TrackBar::TrackBar(Control *parent) : Control(parent)
	{
		type = CONTROL_TRACKBAR;
		
		pressed = false;
		
		SetBounds(6, 6, 110, trackbarSliderHeight + 2);
		
		min = 1;
		max = 10;
		value = 1;

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
	void TrackBar::SetRange(int min, int max)
	{
		this->min = min;
		this->max = max;
		
		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	void TrackBar::GetRange(int *min, int *max) const
	{
		*min = this->min;
		*max = this->max;
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetValue(int value)
	{
		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	int TrackBar::GetValue() const
	{
		return value;
	}
	//---------------------------------------------------------------------------
	KeyDownEvent& TrackBar::GetKeyDownEvent()
	{
		return keyDownEvent;
	}
	//---------------------------------------------------------------------------
	KeyPressEvent& TrackBar::GetKeyPressEvent()
	{
		return keyPressEvent;
	}
	//---------------------------------------------------------------------------
	KeyUpEvent& TrackBar::GetKeyUpEvent()
	{
		return keyUpEvent;
	}
	//---------------------------------------------------------------------------
	ScrollEvent& TrackBar::GetScrollEvent()
	{
		return scrollEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool TrackBar::CanHaveFocus() const
	{
		return isEnabled && isVisible;
	}
	//---------------------------------------------------------------------------
	bool TrackBar::ContainsPoint(const Drawing::Point &point) const
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	const Drawing::Point TrackBar::PointToClient(const Drawing::Point &point) const
	{
		return Drawing::Point(point.Left - clientArea.GetLeft(), point.Top - clientArea.GetTop());
	}
	//---------------------------------------------------------------------------
	void TrackBar::Invalidate()
	{
		if (bounds.GetHeight() != trackbarSliderHeight + 2)
		{
			bounds.SetHeight(trackbarSliderHeight + 2);
		}

		clientArea = bounds;

		sliderMiddle = (int)((value - min) * (float)(clientArea.GetWidth() - trackbarSliderWidth) / (max - min) + trackbarSliderWidth / 2);
		sliderRect = Drawing::Rectangle(clientArea.GetLeft() + sliderMiddle - (trackbarSliderWidth / 2), clientArea.GetTop(), trackbarSliderWidth, trackbarSliderHeight);

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	void TrackBar::SetValueInternal(int value)
	{
		if (value < min || value > max)
		{
			throw Misc::ArgumentOutOfRangeException(L"value", __WFILE__, __LINE__);
		}
		
		if (this->value != value)
		{
			this->value = value;
			
			scrollEvent.Invoke(this);
		}

		Invalidate();
	}
	//---------------------------------------------------------------------------
	int TrackBar::ValueFromPosition(int position) const
	{
		float valuePerPixel = (float)(max - min) / (clientArea.GetWidth() - trackbarSliderWidth);
		return (int)(0.5f + min + valuePerPixel * position);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	bool TrackBar::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}

		if (!isVisible || !isEnabled)
		{
			return false;
		}
	
		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			if (mouse->State == MouseEvent::LeftDown)
			{
				if (Drawing::Rectangle(sliderMiddle - (trackbarSliderWidth / 2), 0, trackbarSliderWidth, trackbarSliderHeight).Contains(mouse->Position)) //SliderRect
				{
					pressed = true;

					if (!isFocused)
					{
						parent->RequestFocus(this);
					}

					MouseEventArgs args(mouse);
					mouseDownEvent.Invoke(this, args);

					return true;
				}

				if (Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight()).Contains(mouse->Position)) //ClientArea
				{
					pressed = true;
					
					if (!isFocused)
					{
						parent->RequestFocus(this);
					}

					SetValueInternal(ValueFromPosition(mouse->Position.X));

					MouseEventArgs args(mouse);
					mouseDownEvent.Invoke(this, args);

					return true;
				}
			}
			else if (mouse->State == MouseEvent::Move)
			{
				if (pressed)
				{
					SetValueInternal(ValueFromPosition(mouse->Position.X));
					
					MouseEventArgs args(mouse);
					mouseMoveEvent.Invoke(this, args);

					return true;
				}
			}
			else if (mouse->State == MouseEvent::LeftUp)
			{
				if (pressed)
				{
					pressed = false;
					
					scrollEvent.Invoke(this);

					MouseEventArgs args(mouse);
					mouseUpEvent.Invoke(this, args);

					return true;
				}
			}
			
			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == IEvent::Keyboard)
		{
			KeyboardEvent *keyboard = (KeyboardEvent*)event;
			if (keyboard->State == KeyboardEvent::KeyDown)
			{
				KeyEventArgs args(keyboard);
				keyDownEvent.Invoke(this, args);
				if (!args.Handled)
				{
					switch (keyboard->KeyCode)
					{
						case Key::Home:
							SetValueInternal(min);
							return true;
						case Key::End:
							SetValueInternal(max);
							return true;
						case Key::Left:
						case Key::Down:
							SetValueInternal(value - 1);
							return true;
						case Key::Right:
						case Key::Up:
							SetValueInternal(value + 1);
							return true;
						case Key::PageDown:
							SetValueInternal(value - std::max(10, (max - min) / 10));
							return true;
						case Key::PageUp:
							SetValueInternal(value + std::max(10, (max - min) / 10));
							return true;
					}
				}
			}
		}
		
		return false;
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
			renderer->Fill(bounds);
		}

		renderer->SetRenderColor(isFocused || mouseOver ? foreColor + Drawing::Color(0, 43, 43, 43) : foreColor);

		int range = max - min;
		int halfWidth = trackbarSliderWidth / 2;
		float space = (bounds.GetWidth() - trackbarSliderWidth) / (float)range;
		if (space < 5.0f)
		{
			space = 5.0f;
			range = (int)((bounds.GetWidth() - trackbarSliderWidth) / space);
		}
		
		for (int i = 0; i < range + 1; ++i)
		{
			renderer->Fill((int)(bounds.GetLeft() + halfWidth + (i * space)), bounds.GetTop() + 6, 1, 5);
		}

		renderer->SetRenderColor(foreColor);
		renderer->Fill(sliderRect);

		if (controls.size() > 0)
		{
			Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
			renderer->SetRenderRectangle(clientArea + renderRect.GetPosition());
			
			ChildRender(renderer);
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}