#include "ScrollBar.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ScrollBar::ScrollBar(Panel *parentPanel)
	{
		type = CONTROL_SCROLLBAR;
		
		ParentPanel = parentPanel;
		
		drag = false;
		showSlider = false;
		
		position = 0;
		pageSize = 1;
		start = 0;
		end = 1;
		
		delayTimestamp = 0;
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
	void ScrollBar::UpdateRects()
	{
		if (bounds.GetWidth() > SCROLLBAR_DEFAULT_BOUNDS_WIDTH)
		{
			bounds.SetWidth(SCROLLBAR_DEFAULT_BOUNDS_WIDTH);
		}
		
		buttonUp = Drawing::Rectangle(bounds.GetLeft() + 3, bounds.GetTop() + 2, SCROLLBAR_DEFAULT_BUTTON_WIDTH, SCROLLBAR_DEFAULT_BUTTON_HEIGHT);
		buttonDown = Drawing::Rectangle(bounds.GetLeft() + 3, bounds.GetBottom() - 2 - SCROLLBAR_DEFAULT_BUTTON_HEIGHT, SCROLLBAR_DEFAULT_BUTTON_WIDTH, SCROLLBAR_DEFAULT_BUTTON_HEIGHT);
		track = Drawing::Rectangle(buttonUp.GetLeft(), buttonUp.GetBottom() + 2, SCROLLBAR_DEFAULT_BUTTON_WIDTH, buttonUp.GetBottom() - buttonDown.GetTop() - 4);
		slider.SetLeft(buttonUp.GetLeft());
		slider.SetWidth(SCROLLBAR_DEFAULT_BUTTON_WIDTH);
		
		UpdateSliderRect();
	}
	//---------------------------------------------------------------------------
	void ScrollBar::UpdateSliderRect()
	{
		if (end - start > pageSize)
		{
			int sliderHeight = track.GetHeight() * pageSize / (end - start);
			if (sliderHeight < SCROLLBAR_MIN_SLIDER_HEIGHT)
			{
				sliderHeight = SCROLLBAR_MIN_SLIDER_HEIGHT;
			}
			int maxPosition = end - start - pageSize;
			
			slider.SetTop(track.GetTop() + (position - start) * ( track.GetHeight() - sliderHeight) / maxPosition);
			slider.SetHeight(sliderHeight);
			showSlider = true;

		}
		else
		{
			slider.SetHeight(0);
			showSlider = false;
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
		static int SliderOffsetY;
	
		if (event == NULL || !visible || !enabled)
		{
			return Event::None;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*) event;
			if (mouse->State == MouseEvent::LeftDown)
			{
				if (buttonUp.Contains(mouse->Position))
				{
					if(position > start)
						position--;
					
					UpdateSliderRect();
					
					return Event::None;
				}
				
				if (buttonDown.Contains(mouse->Position))
				{
					if(position + pageSize < end)
						position++;
					
					UpdateSliderRect();
					
					return Event::None;
				}
				
				if (slider.Contains(mouse->Position))
				{
					drag = true;
					
					SliderOffsetY = mouse->Position.Y - slider.GetTop();
					
					return Event::None;
				}

				if (slider.GetLeft() <= mouse->Position.X && slider.GetRight() > mouse->Position.X)
				{
					if (slider.GetTop() > mouse->Position.Y && track.GetTop() <= mouse->Position.Y)
					{
						Scroll(-(pageSize - 1));
						
						return Event::None;
					}
					else if (slider.GetBottom() <= mouse->Position.Y && track.GetBottom() > mouse->Position.Y)
					{
						Scroll(pageSize - 1);
						return Event::None;
					}
				}
			}
			else if (mouse->State == MouseEvent::LeftUp)
			{
				drag = false;

				UpdateSliderRect();
			}
			else if (mouse->State == MouseEvent::Move)
			{
				if (drag)
				{
					
					//m_rcThumb.bottom += slider.Get mouse->Position.Y - ThumbOffsetY - slider.GetTop();
					slider.SetTop(mouse->Position.Y - SliderOffsetY);
					if (slider.GetTop() < track.GetTop())
					{
						slider.Offset(0, track.GetTop() - slider.GetTop());
					}
					else if(slider.GetBottom() > track.GetBottom())
					{
						slider.Offset(0, track.GetBottom() - slider.GetBottom());
					}

					int maxFirstItem = end - start - pageSize;
					int maxSlider = track.GetHeight() - slider.GetHeight();

					position = start + (slider.GetTop() - track.GetTop() + maxSlider / (maxFirstItem * 2)) * maxFirstItem / maxSlider;
				}
			}
			return Event::None;
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
		//OK
		/*button oben:
		texture2 = new OSHGui::Drawing::Texture(pDevice);
		texture2->Create(OSHGui::Drawing::Size(13, 20));
		texture2->BeginUpdate();

		texture->Fill(0xFF6D6966);
		texture->Clear(0, 0, 1, 1);
		texture->Clear(12, 0, 1, 1);
		texture->Clear(0, 19, 1, 1);
		texture->Clear(12, 19, 1, 1);

		texture->Fill(6, 7, 1, 1, 0xFFBAB8B9);
		texture->Fill(5, 8, 3, 1, 0xFFBAB8B9);
		texture->Fill(4, 9, 5, 1, 0xFFBAB8B9);
		texture->Fill(3, 10, 7, 1, 0xFFBAB8B9);

		texture2->EndUpdate();*/

		/*track:
		Drawing::Size size(22, 173);

		texture = new OSHGui::Drawing::Texture(pDevice);
		texture->Create(size);
		texture->BeginUpdate();
		
		texture->FillGradient(0xFF51504B, 0xFF262523);

		texture->Clear(0, 0, 1, 1);
		texture->Clear(size.Width - 1, 0, 1, 1);
		texture->Clear(0, size.Height - 1, 1, 1);
		texture->Clear(size.Width - 1, size.Height - 1, 1, 1);

		texture2->EndUpdate();*/

		/*slider:
		Drawing::Size size(13, 20);

		texture = new OSHGui::Drawing::Texture(pDevice);
		texture->Create(size);
		texture->BeginUpdate();

		texture->Fill(0xFF5F5E59);
		texture->Clear(0, 0, 1, 1);
		texture->Clear(size.Width - 1, 0, 1, 1);
		texture->Clear(0, size.Height - 1, 1, 1);
		texture->Clear(size.Width - 1, size.Height - 1, 1, 1);

		int temp = size.Height / 2 - 3;
		for (int i = 0; i < 8; i += 2)
		{
			for (int j = 0; j < 8; j += 2)
			{
				texture->Fill(3 + j, temp + i, 1, 1, 0xFFE3E1E2);
			}
		}

		texture2->EndUpdate();*/

		/*button down:
		texture = new OSHGui::Drawing::Texture(pDevice);
		texture->Create(OSHGui::Drawing::Size(13, 20));
		texture->BeginUpdate();

		texture->Fill(0xFF5F5E59);
		texture->Clear(0, 0, 1, 1);
		texture->Clear(12, 0, 1, 1);
		texture->Clear(0, 19, 1, 1);
		texture->Clear(12, 19, 1, 1);

		texture->Fill(6, 12, 1, 1, 0xFFBAB8B9);
		texture->Fill(5, 11, 3, 1, 0xFFBAB8B9);
		texture->Fill(4, 10, 5, 1, 0xFFBAB8B9);
		texture->Fill(3, 9, 7, 1, 0xFFBAB8B9);

		texture2->EndUpdate();*/
	}
	//---------------------------------------------------------------------------
}