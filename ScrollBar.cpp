#include "ScrollBar.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ScrollBar::ScrollBar(Control *parent) : Control(parent)
	{
		type = CONTROL_SCROLLBAR;
		
		drag = false;
		visible = false;
		
		position = 0;
		range = 1;
		pageSize = 1;
		
		delayTimestamp = 0;

		SetBackColor(Drawing::Color(0xFF1E1E1E));
		SetForeColor(Drawing::Color(0xFF373737));
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ScrollBar::SetRange(int range)
	{
		this->range = range;
		Capture();
		UpdateSliderRect();
	}
	//---------------------------------------------------------------------------
	int ScrollBar::GetPosition()
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
	bool ScrollBar::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::Invalidate()
	{
		bounds = Drawing::Rectangle(Parent->GetRight() - SCROLLBAR_DEFAULT_BOUNDS_WIDTH, Parent->GetTop(), SCROLLBAR_DEFAULT_BOUNDS_WIDTH, Parent->GetHeight());

		clientArea = bounds;
		trackRect = Drawing::Rectangle(clientArea.GetLeft(), clientArea.GetTop() + SCROLLBAR_DEFAULT_BUTTON_HEIGHT, SCROLLBAR_DEFAULT_BUTTON_WIDTH, clientArea.GetHeight() - SCROLLBAR_DEFAULT_BUTTON_HEIGHT * 2);
		upButtonRect = Drawing::Rectangle(clientArea.GetLeft(), clientArea.GetTop(), SCROLLBAR_DEFAULT_BUTTON_WIDTH, SCROLLBAR_DEFAULT_BUTTON_HEIGHT);
		downButtonRect = Drawing::Rectangle(clientArea.GetLeft(), clientArea.GetTop() + clientArea.GetHeight() - SCROLLBAR_DEFAULT_BUTTON_HEIGHT, SCROLLBAR_DEFAULT_BUTTON_WIDTH, SCROLLBAR_DEFAULT_BUTTON_HEIGHT);
		
		UpdateSliderRect();
	}
	//---------------------------------------------------------------------------
	void ScrollBar::UpdateSliderRect()
	{
		if (range > pageSize)
		{
			int sliderHeight = (trackRect.GetHeight() - 2) * (pageSize / (float)range);
			if (sliderHeight < SCROLLBAR_MIN_SLIDER_HEIGHT)
			{
				sliderHeight = SCROLLBAR_MIN_SLIDER_HEIGHT;
			}
			
			float positionPerPixel = (trackRect.GetHeight() - 2 - sliderHeight) / ((float)range - pageSize);
			int yPos = trackRect.GetTop() + 1 + positionPerPixel * (position);
			
			sliderRect = Drawing::Rectangle(clientArea.GetLeft(), yPos, SCROLLBAR_DEFAULT_BUTTON_WIDTH, sliderHeight);
			
			visible = true;
		}
		else
		{
			visible = false;
		}
	}
	//---------------------------------------------------------------------------
	void ScrollBar::Capture()
	{
		if (position < 0)
		{
			position = 0;
		}
		else if (position > range - pageSize)
		{
			position = range - pageSize;
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
		if (!visible)
		{
			return false;
		}

		if (index < 0)
		{
			index = 0;
		}

		if (index >= range)
		{
			index = range - 1;
		}

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
	Drawing::Point ScrollBar::PointToClient(const Drawing::Point &point)
	{
		return Drawing::Point(point.Left - (bounds.GetLeft() - Parent->GetLeft()), point.Top);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes ScrollBar::ProcessEvent(Event *event)
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
				if (Drawing::Rectangle(0, 0, SCROLLBAR_DEFAULT_BUTTON_WIDTH, SCROLLBAR_DEFAULT_BUTTON_HEIGHT).Contains(mouse->Position)) //upButton
				{
					if (position > 0)
					{
						--position;
					}
					
					UpdateSliderRect();
					
					return Event::DontContinue;
				}
				
				if (Drawing::Rectangle(0, clientArea.GetHeight() - SCROLLBAR_DEFAULT_BUTTON_HEIGHT, SCROLLBAR_DEFAULT_BUTTON_WIDTH, SCROLLBAR_DEFAULT_BUTTON_HEIGHT).Contains(mouse->Position)) //downButton
				{
					if (position < range - pageSize)
					{
						position++;
					}
					
					UpdateSliderRect();
					
					return Event::DontContinue;
				}
				
				if (Drawing::Rectangle(0, sliderRect.GetTop() - clientArea.GetTop(), SCROLLBAR_DEFAULT_BUTTON_WIDTH, sliderRect.GetHeight()).Contains(mouse->Position)) //sliderRect
				{
					drag = true;
					
					return Event::DontContinue;
				}

				if (Drawing::Rectangle(0, SCROLLBAR_DEFAULT_BUTTON_HEIGHT, SCROLLBAR_DEFAULT_BUTTON_WIDTH, clientArea.GetHeight() - SCROLLBAR_DEFAULT_BUTTON_HEIGHT * 2).Contains(mouse->Position)) //trackRect
				{
					if (sliderRect.GetTop() - clientArea.GetTop() > mouse->Position.Y)
					{
						Scroll(-(pageSize - 1));
						
						return Event::DontContinue;
					}
					else if (sliderRect.GetBottom() - clientArea.GetBottom() < mouse->Position.Y)
					{
						Scroll(pageSize - 1);
						
						return Event::DontContinue;
					}
				}
			}
			else if (mouse->State == MouseEvent::LeftUp)
			{
				if (drag)
				{
					drag = false;

					UpdateSliderRect();
					
					return Event::DontContinue;
				}
			}
			else if (mouse->State == MouseEvent::Move)
			{
				if (drag)
				{
					float positionPerPixel = (float)(range - pageSize) / (trackRect.GetHeight() - 2 - sliderRect.GetHeight());

					int yPos = mouse->Position.Y - SCROLLBAR_DEFAULT_BUTTON_HEIGHT - sliderRect.GetHeight() / 2;
					if (yPos < 0)
					{
						yPos = 0;
					}
					else if (yPos + sliderRect.GetHeight() + trackRect.GetTop() > downButtonRect.GetTop())
					{
						yPos = downButtonRect.GetTop() - sliderRect.GetHeight() - trackRect.GetTop() - 1;
					}

					position = yPos * positionPerPixel;
					if (position > range - pageSize)
					{
						position = range - pageSize;
					}
					
					sliderRect.SetTop(upButtonRect.GetBottom() + yPos + 1);

					return Event::DontContinue;
				}
			}
			else if (mouse->State == MouseEvent::Scroll)
			{
				if (!drag)
				{
					Scroll(mouse->Delta);
				}
				
				return Event::DontContinue;
			}
			mouse->Position = mousePositionBackup;
			
			return Event::Continue;
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void ScrollBar::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}

		renderer->SetRenderColor(foreColor);
		renderer->Fill(clientArea.GetLeft() - 2, clientArea.GetTop(), 1, clientArea.GetHeight());

		for (int i = 0; i < 4; ++i)
		{
			//upButton
			renderer->Fill(upButtonRect.GetLeft() + 6 - i, upButtonRect.GetTop() + 8 + i, 1 + i * 2, 1);
			//downButton
			renderer->Fill(downButtonRect.GetLeft() + 6 - i, downButtonRect.GetBottom() - 9 - i, 1 + i * 2, 1);
		}

		renderer->SetRenderColor(backColor);
		renderer->Fill(sliderRect.GetLeft() + 1, sliderRect.GetTop() + 1, sliderRect.GetWidth() - 2, sliderRect.GetHeight() - 2);
		renderer->SetRenderColor(foreColor);
		renderer->Fill(sliderRect.GetLeft(), sliderRect.GetTop() + 1, 1, sliderRect.GetHeight() - 3);
		renderer->Fill(sliderRect.GetLeft() + 1, sliderRect.GetTop(), sliderRect.GetWidth() - 3, 1);

		int sliderHalfHeight = sliderRect.GetTop() + sliderRect.GetHeight() / 2 - 3;
		for (int i = 0; i < 3; ++i)
		{
			renderer->Fill(sliderRect.GetLeft() + 5, sliderHalfHeight + i * 3, 5, 1);
		}

		renderer->SetRenderColor(foreColor - Drawing::Color(0, 50, 50, 50));
		renderer->Fill(sliderRect.GetRight() - 1, sliderRect.GetTop() + 1, 1, sliderRect.GetHeight() - 2);
		renderer->Fill(sliderRect.GetLeft() + 1, sliderRect.GetBottom() - 1, sliderRect.GetWidth() - 2, 1);
	}
	//---------------------------------------------------------------------------
}