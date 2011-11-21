#include "ScrollBar.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	const int ScrollBar::scrollbarMinSliderHeight = 25;
	const int ScrollBar::scrollbarDefaultBoundsWidth = 14;
	const int ScrollBar::scrollbarDefaultButtonWidth = 14;
	const int ScrollBar::scrollbarDefaultButtonHeight = 18;
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ScrollBar::ScrollBar(Control *parent) : Control(parent)
	{
		type = CONTROL_SCROLLBAR;
		
		drag = false;
		isVisible = false;
				
		position = 0;
		range = 1;
		pageSize = 1;
		
		SetBackColor(Drawing::Color(0xFF585552));
		SetForeColor(Drawing::Color(0xFFAFADAD));
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
	int ScrollBar::GetRange() const
	{
		return range;
	}
	//---------------------------------------------------------------------------
	int ScrollBar::GetPosition() const
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
	bool ScrollBar::ContainsPoint(const Drawing::Point &point) const
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void ScrollBar::Invalidate()
	{
		bounds = Drawing::Rectangle(parent->GetRight() - scrollbarDefaultBoundsWidth - 2, parent->GetTop(), scrollbarDefaultBoundsWidth, parent->GetHeight());

		clientArea = bounds;
		trackRect = Drawing::Rectangle(clientArea.GetLeft(), clientArea.GetTop() + scrollbarDefaultButtonHeight, scrollbarDefaultButtonWidth, clientArea.GetHeight() - scrollbarDefaultButtonHeight * 2);
		upButtonRect = Drawing::Rectangle(clientArea.GetLeft(), clientArea.GetTop(), scrollbarDefaultButtonWidth, scrollbarDefaultButtonHeight);
		downButtonRect = Drawing::Rectangle(clientArea.GetLeft(), clientArea.GetTop() + clientArea.GetHeight() - scrollbarDefaultButtonHeight, scrollbarDefaultButtonWidth, scrollbarDefaultButtonHeight);
		
		UpdateSliderRect();
	}
	//---------------------------------------------------------------------------
	void ScrollBar::UpdateSliderRect()
	{
		if (range > pageSize)
		{
			int sliderHeight = (int)((trackRect.GetHeight() - 2) * (pageSize / (float)range));
			if (sliderHeight < scrollbarMinSliderHeight)
			{
				sliderHeight = scrollbarMinSliderHeight;
			}
			
			float positionPerPixel = (trackRect.GetHeight() - 2 - sliderHeight) / ((float)range - pageSize);
			int yPos = (int)(trackRect.GetTop() + 1 + positionPerPixel * (position));
			
			sliderRect = Drawing::Rectangle(clientArea.GetLeft(), yPos, scrollbarDefaultButtonWidth, sliderHeight);
			
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
		if (!isVisible)
		{
			return false;
		}

		if (index < 0 || index >= range)
		{
			throw Misc::ArgumentOutOfRangeException(L"index", __WFILE__, __LINE__);
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
		return Drawing::Point(point.Left - (bounds.GetLeft() - parent->GetLeft()), point.Top);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	bool ScrollBar::ProcessEvent(IEvent *event)
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
			MouseMessage *mouse = (MouseMessage*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			if (mouse->State == MouseMessage::LeftDown)
			{
				if (Drawing::Rectangle(0, 0, scrollbarDefaultButtonWidth, scrollbarDefaultButtonHeight).Contains(mouse->Position)) //upButton
				{
					if (position > 0)
					{
						--position;
					}
					
					UpdateSliderRect();
					
					return true;
				}
				
				if (Drawing::Rectangle(0, clientArea.GetHeight() - scrollbarDefaultButtonHeight, scrollbarDefaultButtonWidth, scrollbarDefaultButtonHeight).Contains(mouse->Position)) //downButton
				{
					if (position < range - pageSize)
					{
						++position;
					}
					
					UpdateSliderRect();
					
					return true;
				}
				
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
	//---------------------------------------------------------------------------
	void ScrollBar::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		renderer->SetRenderColor(foreColor);
		for (int i = 0; i < 4; ++i)
		{
			//upButton
			renderer->Fill(upButtonRect.GetLeft() + 7 - i, upButtonRect.GetTop() + 8 + i, 1 + i * 2, 1);
			//downButton
			renderer->Fill(downButtonRect.GetLeft() + 7 - i, downButtonRect.GetBottom() - 9 - i, 1 + i * 2, 1);
		}

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
}