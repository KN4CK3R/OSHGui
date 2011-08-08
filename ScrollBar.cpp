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
		showScrollBar = false;
		
		position = 0;
		pageSize = 1;
		start = 0;
		end = 1;
		
		delayTimestamp = 0;

		SetBackColor(Drawing::Color(0xFF1E1E1E));
		SetForeColor(Drawing::Color(0xFF373737));
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ScrollBar::SetRange(int start, int end)
	{
		this->start = start;
		this->end = end;
		Capture();
		UpdateSliderRect();
	}
	//---------------------------------------------------------------------------
	void ScrollBar::SetPosition(int position)
	{
		this->position = position;
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
		pageSize = pageSize;
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
		bounds = Drawing::Rectangle(Parent->GetRight() - SCROLLBAR_DEFAULT_BOUNDS_WIDTH - 3, 0, SCROLLBAR_DEFAULT_BOUNDS_WIDTH, Parent->GetHeight());

		clientArea = Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight());
		trackRect = Drawing::Rectangle(0, SCROLLBAR_DEFAULT_BUTTON_HEIGHT, SCROLLBAR_DEFAULT_BUTTON_WIDTH, clientArea.GetHeight() - SCROLLBAR_DEFAULT_BUTTON_HEIGHT * 2);
		upButtonRect = Drawing::Rectangle(0, 0, SCROLLBAR_DEFAULT_BUTTON_WIDTH, SCROLLBAR_DEFAULT_BUTTON_HEIGHT);
		downButtonRect = Drawing::Rectangle(0, clientArea.GetHeight() - SCROLLBAR_DEFAULT_BUTTON_HEIGHT, SCROLLBAR_DEFAULT_BUTTON_WIDTH, SCROLLBAR_DEFAULT_BUTTON_HEIGHT);
				
		UpdateSliderRect();
	}
	//---------------------------------------------------------------------------
	void ScrollBar::UpdateSliderRect()
	{
		if (end - start > pageSize)
		{
			int sliderHeight = (clientArea.GetHeight() - 2 * 22) * pageSize / (end - start);
			if (sliderHeight < SCROLLBAR_MIN_SLIDER_HEIGHT)
			{
				sliderHeight = SCROLLBAR_MIN_SLIDER_HEIGHT;
			}
			int maxPosition = end - start - pageSize;
			
			sliderRect = Drawing::Rectangle(0, SCROLLBAR_DEFAULT_BUTTON_HEIGHT + ((clientArea.GetHeight() - 2 * 22) / maxPosition) * position, SCROLLBAR_DEFAULT_BUTTON_WIDTH, sliderHeight);
			showScrollBar = true;
		}
		else
		{
			showScrollBar = false;
		}
	}
	//---------------------------------------------------------------------------
	void ScrollBar::Capture()
	{
		if (position < start || end - start <= pageSize)
		{
			position = start;
		}
		else if (position + pageSize > end)
		{
			position = end - pageSize;
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
	void ScrollBar::ShowItem(int index)
	{
		if (index < 0)
		{
			index = 0;
		}

		if (index >= end)
		{
			index = end - 1;
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
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes ScrollBar::ProcessEvent(Event *event)
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
				if (upButtonRect.Contains(mouse->Position))
				{
					if (position > start)
					{
						--position;
					}
					
					UpdateSliderRect();
					
					return Event::DontContinue;
				}
				
				if (downButtonRect.Contains(mouse->Position))
				{
					if (position + pageSize < end)
					{
						position++;
					}
					
					UpdateSliderRect();
					
					return Event::DontContinue;
				}
				
				if (sliderRect.Contains(mouse->Position))
				{
					drag = true;
					
					SliderOffsetY = mouse->Position.Y - sliderRect.GetTop();
					
					return Event::DontContinue;
				}

				if (trackRect.contains(mouse->Position))
				{
					if (sliderRect.GetTop() > mouse->Position.Y)
					{
						Scroll(-(pageSize - 1));
						
						return Event::DontContinue;
					}
					else if (sliderRect.GetBottom() < mouse->Position.Y)
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
					float valuePerPixel = (float)(end - start) / (trackRect.GetHeight() - sliderRect.GetHeight());
					int value = (int)(0.5f + start + valuePerPixel * mouse->Position.Y);
					
					if (value <= SCROLLBAR_DEFAULT_BUTTON_HEIGHT)
					{
						value = SCROLLBAR_DEFAULT_BUTTON_HEIGHT + 1;
					}
					else if (value + sliderRect.GetHeight() + 1 >= trackRect.GetBottom())
					{
						value = trackRect.GetBottom() - sliderRect.GetHeight() - 1;
					}
					
					sliderRect.SetTop(value);
					
					position = start + valuePerPixel * value;
				}
				return Event::DontContinue;
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
		else if (event->Type == Event::System)
		{
			SystemEvent *system = (SystemEvent*)event;
			if (system->State == SystemEvent::CaptureChanged)
			{
				drag = false;
			}
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void ScrollBar::Render(Drawing::IRenderer *renderer)
	{
		if (!showScrollBar)
		{
			return;
		}

		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		Drawing::Rectangle tmp = clientArea + bounds.GetPosition() + renderRect.GetPosition();
		renderer->SetRenderRectangle(tmp);

		renderer->SetRenderColor(foreColor);
		for (int i = 0; i < 4; ++i)
		{
			//upButton
			renderer->Fill(6 - i, 8 + i, 1 + i * 2, 1);
			//downButton
			renderer->Fill(6 - i, clientArea.GetHeight() - 9 - i, 1 + i * 2, 1);
		}
		
		renderer->SetRenderRectangle(tmp + sliderRect.GetPosition());

		renderer->SetRenderColor(backColor);
		renderer->Fill(1, 1, sliderRect.GetWidth() - 2, sliderRect.GetHeight() - 2);
		renderer->SetRenderColor(foreColor);
		renderer->Fill(0, 1, 1, sliderRect.GetHeight() - 3);
		renderer->Fill(1, 0, sliderRect.GetWidth() - 3, 1);

		int sliderHalfHeight = sliderRect.GetHeight() / 2 - 3;
		for (int i = 0; i < 3; ++i)
		{
			renderer->Fill (5, sliderHalfHeight + i * 3, 5, 1);
		}

		renderer->SetRenderColor(foreColor - Drawing::Color(0, 50, 50, 50));
		renderer->Fill(sliderRect.GetWidth() - 1, 1, 1, sliderRect.GetHeight() - 2);
		renderer->Fill(1, sliderRect.GetHeight() - 1, sliderRect.GetWidth() - 2, 1);

		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}