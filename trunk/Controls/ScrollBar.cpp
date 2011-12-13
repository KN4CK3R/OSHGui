#include "ScrollBar.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::Size ScrollBar::DefaultSize(14, 110);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ScrollBar::ScrollBar()
	{
		type = CONTROL_SCROLLBAR;
		
		drag = false;
				
		value = 0;
		
		upButton = new ScrollBarButton(0);
		upButton->GetClickEvent() += ClickEventHandler([this](Control *control)
		{
			if (value > 0)
			{
				SetValueInternal(value - 1);
			}
		});
		AddSubControl(upButton);
		downButton = new ScrollBarButton(1);
		downButton->GetClickEvent() += ClickEventHandler([this](Control *control)
		{
			if (value < maximum)
			{
				SetValueInternal(value + 1);
			}
		});
		AddSubControl(downButton);

		SetSize(DefaultSize);
		trackLocation = Drawing::Point(0, upButton->GetBottom() + 1);

		SetBackColor(Drawing::Color(0xFF585552));
		SetForeColor(Drawing::Color(0xFFAFADAD));

		SetMaximum(0); //indirect init
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ScrollBar::SetSize(const Drawing::Size &size)
	{
		ContainerControl::SetSize(size);

		upButton->SetSize(Drawing::Size(size.Width, upButton->GetHeight()));
		downButton->SetSize(Drawing::Size(size.Width, downButton->GetHeight()));

		downButton->SetLocation(0, size.Height - downButton->GetHeight());

		trackSize = Drawing::Size(size.Width, size.Height - 2 - upButton->GetHeight() * 2);

		sliderSize.Width = size.Width;
		sliderSize.Height = trackSize.Height / (maximum + 1);
		if (sliderSize.Height < MinimumSliderHeight)
		{
			sliderSize.Height = MinimumSliderHeight;
		}

		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::SetForeColor(const Drawing::Color &color)
	{
		ContainerControl::SetForeColor(color);

		upButton->SetForeColor(color);
		downButton->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::SetMaximum(int maximum)
	{
		if (maximum < 0)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentOutOfRangeException("maximum", __FILE__, __LINE__);
			#endif
			return;
		}

		this->maximum = maximum;

		sliderSize.Height = trackSize.Height / (maximum + 1);
		if (sliderSize.Height < MinimumSliderHeight)
		{
			sliderSize.Height = MinimumSliderHeight;
		}

		SetValueInternal(value > maximum ? maximum : value);
	}
	//---------------------------------------------------------------------------
	int ScrollBar::GetMaximum() const
	{
		return maximum;
	}
	//---------------------------------------------------------------------------
	void ScrollBar::SetValue(int value)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (value < 0 || value > maximum)
		{
			throw Misc::ArgumentOutOfRangeException("value", __FILE__, __LINE__);
		}
		#endif

		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	int ScrollBar::GetValue() const
	{
		return value;
	}
	//---------------------------------------------------------------------------
	ScrollEvent& ScrollBar::GetScrollEvent()
	{
		return scrollEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void ScrollBar::SetValueInternal(int value)
	{
		pixelsPerTick = (trackSize.Height - sliderSize.Height) / (maximum > 0 ? (float)maximum : 1.0f);

		if (value < 0)
		{
			value = 0;
		}
		if (value > maximum)
		{
			value = maximum;
		}
		
		if (this->value != value)
		{
			ScrollEventArgs args(value, this->value);

			this->value = value;
			
			scrollEvent.Invoke(this, args);
		}

		sliderLocation.Top = trackLocation.Top + value * pixelsPerTick;
		sliderAbsoluteLocation.Top = absoluteLocation.Top + sliderLocation.Top;
	}
	//---------------------------------------------------------------------------
	bool ScrollBar::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::CalculateAbsoluteLocation()
	{
		ContainerControl::CalculateAbsoluteLocation();

		sliderAbsoluteLocation = absoluteLocation + sliderLocation;
		trackAbsoluteLocation = absoluteLocation + trackLocation;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void ScrollBar::OnMouseDown(const MouseMessage &mouse)
	{
		Control::OnMouseDown(mouse);

		if (Intersection::TestRectangle(sliderAbsoluteLocation, sliderSize, mouse.Position))
		{
			drag = true;
			OnGotMouseCapture();
		}
	}
	//---------------------------------------------------------------------------
	void ScrollBar::OnMouseMove(const MouseMessage &mouse)
	{
		Control::OnMouseMove(mouse);

		if (drag)
		{
			if (maximum > 1)
			{
				float valuePerPixel = (float)maximum / (trackSize.Height - sliderSize.Height);

				int yPos = mouse.Position.Y - trackAbsoluteLocation.Top - sliderSize.Height / 2;
				if (yPos < 0)
				{
					yPos = 0;
				}
				else if (yPos + sliderSize.Height + trackLocation.Top > trackSize.Height)
				{
					yPos = trackSize.Height - sliderSize.Height;
				}

				SetValueInternal(yPos * valuePerPixel + 0.5f);
			}
		}
	}
	//---------------------------------------------------------------------------
	void ScrollBar::OnMouseUp(const MouseMessage &mouse)
	{
		Control::OnMouseUp(mouse);

		if (drag)
		{
			drag = false;
			OnLostMouseCapture();
		}
	}
	//---------------------------------------------------------------------------
	void ScrollBar::OnMouseClick(const MouseMessage &mouse)
	{
		Control::OnMouseClick(mouse);

		if (!drag && maximum > 1)
		{
			if (Intersection::TestRectangle(trackAbsoluteLocation, trackSize, mouse.Position))
			{
				float valuePerPixel = (float)maximum / (trackSize.Height - sliderSize.Height);
				int yPos = mouse.Position.Y - trackAbsoluteLocation.Top - sliderSize.Height / 2;
				SetValueInternal(yPos * valuePerPixel + 0.5f);
			}
		}
	}
	//---------------------------------------------------------------------------
	/*bool ScrollBar::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException("event", __FILE__, __LINE__);
		}

		if (!isVisible || !isEnabled)
		{
			return false;
		}
	
		if (event->Type == IEvent::Mouse)
		{
			MouseMessage *mouse = (MouseMessage*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			if (mouse->State == MouseMessage::LeftDown)
			{				
				if (Drawing::Rectangle(0, sliderRect.GetTop() - clientArea.GetTop(), scrollbarDefaultButtonWidth, sliderRect.GetHeight()).Contains(mouse->Position)) //sliderRect
				{
					drag = true;
					
					return true;
				}

				if (Drawing::Rectangle(0, scrollbarDefaultButtonHeight, scrollbarDefaultButtonWidth, clientArea.GetHeight() - scrollbarDefaultButtonHeight * 2).Contains(mouse->Position)) //trackRect
				{
					if (sliderRect.GetTop() - clientArea.GetTop() > mouse->Position.Y)
					{
						Scroll(-(pageSize - 1));
						
						return true;
					}
					else if (sliderRect.GetBottom() - clientArea.GetBottom() < mouse->Position.Y)
					{
						Scroll(pageSize - 1);
						
						return true;
					}
				}
			}
			else if (mouse->State == MouseMessage::LeftUp)
			{
				if (drag)
				{
					drag = false;

					UpdateSliderRect();
					
					return true;
				}
			}
			else if (mouse->State == MouseMessage::Move)
			{
				if (drag)
				{
					float positionPerPixel = (float)(range - pageSize) / (trackRect.GetHeight() - 2 - sliderRect.GetHeight());

					int yPos = mouse->Position.Y - scrollbarDefaultButtonHeight - sliderRect.GetHeight() / 2;
					if (yPos < 0)
					{
						yPos = 0;
					}
					else if (yPos + sliderRect.GetHeight() + trackRect.GetTop() > downButtonRect.GetTop())
					{
						yPos = downButtonRect.GetTop() - sliderRect.GetHeight() - trackRect.GetTop() - 1;
					}

					position = (int)(yPos * positionPerPixel);
					if (position > range - pageSize)
					{
						position = range - pageSize;
					}
					
					sliderRect.SetTop(upButtonRect.GetBottom() + yPos + 1);

					return true;
				}
			}
			else if (mouse->State == MouseMessage::Scroll)
			{
				if (!drag)
				{
					Scroll(mouse->Delta);
				}
				
				return true;
			}
			mouse->Position = mousePositionBackup;
			
			return false;
		}
		
		return false;
	}
	//---------------------------------------------------------------------------*/
	void ScrollBar::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		upButton->Render(renderer);
		downButton->Render(renderer);

		renderer->SetRenderColor(backColor);
		renderer->Fill(sliderAbsoluteLocation.Left + 1, sliderAbsoluteLocation.Top + 1, sliderSize.Width - 2, sliderSize.Height - 2);
		renderer->Fill(sliderAbsoluteLocation.Left + sliderSize.Width - 1, sliderAbsoluteLocation.Top + 1, 1, sliderSize.Height - 2);
		renderer->Fill(sliderAbsoluteLocation.Left + 1, sliderAbsoluteLocation.Top + sliderSize.Height - 1, sliderSize.Width - 2, 1);
		renderer->Fill(sliderAbsoluteLocation.Left, sliderAbsoluteLocation.Top + 1, 1, sliderSize.Height - 2);
		renderer->Fill(sliderAbsoluteLocation.Left + 1, sliderAbsoluteLocation.Top, sliderSize.Width - 2, 1);
		renderer->SetRenderColor(isInside ? foreColor + Drawing::Color(0, 50, 50, 50) : foreColor);
		int sliderHalfHeight = sliderAbsoluteLocation.Top + sliderSize.Height / 2 - 3;
		int sliderLeftPos = sliderAbsoluteLocation.Left + 5;
		for (int i = 0; i < 3; ++i)
		{
			renderer->Fill(sliderLeftPos, sliderHalfHeight + i * 3, 5, 1);
		}
	}
	//---------------------------------------------------------------------------
	//ScrollBar::ScrollBarButton
	//---------------------------------------------------------------------------
	const Drawing::Size ScrollBar::ScrollBarButton::DefaultButtonSize(14, 14);
	//---------------------------------------------------------------------------
	ScrollBar::ScrollBarButton::ScrollBarButton(int direction)
		: Control()
	{
		this->direction = direction;

		SetLocation(0, 0);
		SetSize(DefaultButtonSize);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::ScrollBarButton::SetSize(const Drawing::Size &size)
	{
		Control::SetSize(size);

		iconLocation = absoluteLocation + Drawing::Point(size.Width / 2, size.Height / 2);
	}
	//---------------------------------------------------------------------------
	bool ScrollBar::ScrollBarButton::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::ScrollBarButton::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();

		iconLocation = absoluteLocation + Drawing::Point(GetWidth() / 2, GetHeight() / 2);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::ScrollBarButton::Render(Drawing::IRenderer *renderer)
	{
		renderer->SetRenderColor(isInside ? foreColor + Drawing::Color(0, 50, 50, 50) : foreColor);
		if (direction == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				renderer->Fill(iconLocation.Left - i, iconLocation.Top + i, 1 + i * 2, 1);
			}
		}
		else
		{
			for (int i = 0; i < 4; ++i)
			{
				renderer->Fill(iconLocation.Left - i, iconLocation.Top - i, 1 + i * 2, 1);
			}
		}
	}
	//---------------------------------------------------------------------------
	//ScrollBar::ScrollBarSliderTrack
	//---------------------------------------------------------------------------
	/*ScrollBar::ScrollBarSliderTrack::ScrollBarSliderTrack()
	{

	}*/
	//---------------------------------------------------------------------------
}