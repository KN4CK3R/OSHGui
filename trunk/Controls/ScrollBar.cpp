#include "ScrollBar.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ScrollBar::ScrollBar()
	{
		type = CONTROL_SCROLLBAR;
		
		drag = false;
		//isVisible = false;
				
		value = 0;
		maximum = 1;
		pageSize = 1;
		
		upButton = new ScrollBarButton(0);
		upButton->GetClickEvent() += ClickEventHandler([this](Control *control)
		{
			if (value > 0)
			{
				SetValueInternal(value - 1);
			}
					
			UpdateSliderRect();
		});
		AddSubControl(upButton);
		downButton = new ScrollBarButton(1);
		downButton->GetClickEvent() += ClickEventHandler([this](Control *control)
		{
			if (value < maximum)
			{
				SetValueInternal(value + 1);
			}
					
			UpdateSliderRect();
		});
		AddSubControl(downButton);

		trackLocation = Drawing::Point(0, upButton->GetBottom() + 1);

		SetBackColor(Drawing::Color(0xFF585552));
		SetForeColor(Drawing::Color(0xFFAFADAD));
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
		this->maximum = maximum;

		sliderSize.Height = (trackRect.GetHeight() - 2) * (pageSize / (float)maximum);
		if (sliderSize.Height < MinimumSliderHeight)
		{
			sliderSize.Height = MinimumSliderHeight;
		}

		SetValueInternal(value);
	}
	//---------------------------------------------------------------------------
	int ScrollBar::GetMaximum() const
	{
		return maximum;
	}
	//---------------------------------------------------------------------------
	int ScrollBar::GetValue() const
	{
		return position;
	}
	//---------------------------------------------------------------------------
	void ScrollBar::SetPageSize(int pageSize)
	{
		this->pageSize = pageSize;
		Capture();
		UpdateSliderRect();
	}
	//---------------------------------------------------------------------------
	int ScrollBar::GetPageSize()
	{
		return pageSize;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void ScrollBar::SetValueInternal(int value)
	{
		pixelsPerTick = (GetHeight() - sliderSize.Height) / maximum;

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

			sliderLocation.Top = value * pixelsPerTick;
			sliderAbsoluteLocation.Top = absoluteLocation.Top + sliderLocation.Top;
		}
	}
	//---------------------------------------------------------------------------
	bool ScrollBar::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	/*void ScrollBar::Invalidate()
	{
		bounds = Drawing::Rectangle(parent->GetRight() - scrollbarDefaultBoundsWidth - 2, parent->GetTop(), scrollbarDefaultBoundsWidth, parent->GetHeight());

		clientArea = bounds;
		trackRect = Drawing::Rectangle(clientArea.GetLeft(), clientArea.GetTop() + scrollbarDefaultButtonHeight, scrollbarDefaultButtonWidth, clientArea.GetHeight() - scrollbarDefaultButtonHeight * 2);
		upButtonRect = Drawing::Rectangle(clientArea.GetLeft(), clientArea.GetTop(), scrollbarDefaultButtonWidth, scrollbarDefaultButtonHeight);
		downButtonRect = Drawing::Rectangle(clientArea.GetLeft(), clientArea.GetTop() + clientArea.GetHeight() - scrollbarDefaultButtonHeight, scrollbarDefaultButtonWidth, scrollbarDefaultButtonHeight);
		
		UpdateSliderRect();
	}
	//---------------------------------------------------------------------------*/
	void ScrollBar::UpdateSliderRect()
	{
		if (maximum > pageSize)
		{
			int sliderHeight = (int)((trackRect.GetHeight() - 2) * (pageSize / (float)maximum));
			if (sliderHeight < MinimumSliderHeight)
			{
				sliderHeight = MinimumSliderHeight;
			}
			
			float positionPerPixel = (trackRect.GetHeight() - 2 - sliderHeight) / ((float)maximum - pageSize);
			int yPos = (int)(trackRect.GetTop() + 1 + positionPerPixel * (position));
			
			sliderRect = Drawing::Rectangle(absoluteLocation.Left, yPos, GetWidth(), sliderHeight);
			
			isVisible = true;
		}
		else
		{
			isVisible = false;
		}
	}
	//---------------------------------------------------------------------------
	void ScrollBar::Capture()
	{
		if (position < 0)
		{
			position = 0;
		}
		else if (position > maximum - pageSize)
		{
			position = maximum - pageSize;
		}
	}
	//---------------------------------------------------------------------------
	void ScrollBar::Scroll(int delta)
	{
		position += delta;

		Capture();

		UpdateSliderRect();
	}
	//---------------------------------------------------------------------------
	bool ScrollBar::ShowItem(int index)
	{
		if (!isVisible)
		{
			return false;
		}

		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (index < 0 || index >= maximum)
		{
			throw Misc::ArgumentOutOfRangeException("index", __FILE__, __LINE__);
		}
		#endif

		if (position > index)
		{
			position = index;
		}
		else if (position + pageSize <= index)
		{
			position = index - pageSize + 1;
		}

		UpdateSliderRect();

		return true;
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
			float positionPerPixel = (float)maximum / (trackSize.Height - 2 - sliderSize.Height);

			int yPos = mouse.Position.Y - upButton->GetHeight() - sliderSize.Height / 2;
			if (yPos < 0)
			{
				yPos = 0;
			}
			else if (yPos + sliderSize.Height + trackLocation.Top > downButton->GetTop())
			{
				yPos = downButton->GetTop() - sliderSize.Height - trackLocation.Top - 1;
			}

			SetValueInternal(yPos * positionPerPixel);
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

		if (!drag)
		{
			if (Intersection::TestRectangle(trackAbsoluteLocation, trackSize, mouse.Position))
			{
				value = value;
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

		renderer->SetRenderColor(backColor);
		renderer->Fill(absoluteLocation, size);

		upButton->Render(renderer);
		downButton->Render(renderer);

		renderer->SetRenderColor(backColor);
		renderer->Fill(sliderRect.GetLeft() + 1, sliderRect.GetTop() + 1, sliderRect.GetWidth() - 2, sliderRect.GetHeight() - 2);
		renderer->Fill(sliderRect.GetRight() - 1, sliderRect.GetTop() + 1, 1, sliderRect.GetHeight() - 2);
		renderer->Fill(sliderRect.GetLeft() + 1, sliderRect.GetBottom() - 1, sliderRect.GetWidth() - 2, 1);
		renderer->Fill(sliderRect.GetLeft(), sliderRect.GetTop() + 1, 1, sliderRect.GetHeight() - 2);
		renderer->Fill(sliderRect.GetLeft() + 1, sliderRect.GetTop(), sliderRect.GetWidth() - 2, 1);
		renderer->SetRenderColor(foreColor);
		int sliderHalfHeight = sliderRect.GetTop() + sliderRect.GetHeight() / 2 - 3;
		for (int i = 0; i < 3; ++i)
		{
			renderer->Fill(sliderRect.GetLeft() + 5, sliderHalfHeight + i * 3, 5, 1);
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