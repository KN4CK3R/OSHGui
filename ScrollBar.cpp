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

		SetBackColor(Drawing::Color(0xFFF0F0F0));
		SetForeColor(Drawing::Color(0xFFD1CFCD));
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
		
		upButtonRect = Drawing::Rectangle(bounds.GetLeft() + 3, bounds.GetTop() + 2, SCROLLBAR_DEFAULT_BUTTON_WIDTH, SCROLLBAR_DEFAULT_BUTTON_HEIGHT);
		downButtonRect = Drawing::Rectangle(bounds.GetLeft() + 3, bounds.GetBottom() - 2 - SCROLLBAR_DEFAULT_BUTTON_HEIGHT, SCROLLBAR_DEFAULT_BUTTON_WIDTH, SCROLLBAR_DEFAULT_BUTTON_HEIGHT);
		trackRect = Drawing::Rectangle(upButtonRect.GetLeft(), upButtonRect.GetBottom() + 2, SCROLLBAR_DEFAULT_BUTTON_WIDTH, upButtonRect.GetBottom() - downButtonRect.GetTop() - 4);
		sliderRect.SetLeft(upButtonRect.GetLeft());
		sliderRect.SetWidth(SCROLLBAR_DEFAULT_BUTTON_WIDTH);
		
		UpdateSliderRect();
	}
	//---------------------------------------------------------------------------
	void ScrollBar::UpdateSliderRect()
	{
		if (end - start > pageSize)
		{
			int sliderHeight = trackRect.GetHeight() * pageSize / (end - start);
			if (sliderHeight < SCROLLBAR_MIN_SLIDER_HEIGHT)
			{
				sliderHeight = SCROLLBAR_MIN_SLIDER_HEIGHT;
			}
			int maxPosition = end - start - pageSize;
			
			sliderRect.SetTop(trackRect.GetTop() + (position - start) * ( trackRect.GetHeight() - sliderHeight) / maxPosition);
			sliderRect.SetHeight(sliderHeight);
			showSlider = true;

		}
		else
		{
			sliderRect.SetHeight(0);
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
				if (upButtonRect.Contains(mouse->Position))
				{
					if(position > start)
						position--;
					
					UpdateSliderRect();
					
					return Event::None;
				}
				
				if (downButtonRect.Contains(mouse->Position))
				{
					if(position + pageSize < end)
						position++;
					
					UpdateSliderRect();
					
					return Event::None;
				}
				
				if (sliderRect.Contains(mouse->Position))
				{
					drag = true;
					
					SliderOffsetY = mouse->Position.Y - sliderRect.GetTop();
					
					return Event::None;
				}

				if (sliderRect.GetLeft() <= mouse->Position.X && sliderRect.GetRight() > mouse->Position.X)
				{
					if (sliderRect.GetTop() > mouse->Position.Y && trackRect.GetTop() <= mouse->Position.Y)
					{
						Scroll(-(pageSize - 1));
						
						return Event::None;
					}
					else if (sliderRect.GetBottom() <= mouse->Position.Y && trackRect.GetBottom() > mouse->Position.Y)
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
					sliderRect.SetTop(mouse->Position.Y - SliderOffsetY);
					if (sliderRect.GetTop() < trackRect.GetTop())
					{
						sliderRect.Offset(0, trackRect.GetTop() - sliderRect.GetTop());
					}
					else if(sliderRect.GetBottom() > trackRect.GetBottom())
					{
						sliderRect.Offset(0, trackRect.GetBottom() - sliderRect.GetBottom());
					}

					int maxFirstItem = end - start - pageSize;
					int maxSlider = trackRect.GetHeight() - sliderRect.GetHeight();

					position = start + (sliderRect.GetTop() - trackRect.GetTop() + maxSlider / (maxFirstItem * 2)) * maxFirstItem / maxSlider;
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
		if (needRepaint)
		{
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture()); //up button
				texture.Add(renderer->CreateNewTexture()); //track
				texture.Add(renderer->CreateNewTexture()); //slider
				texture.Add(renderer->CreateNewTexture()); //down button
			}
			
			Drawing::ITexture *upButton = texture.Get(0);
			{
				upButton->Create(upButtonRect.GetSize());
				upButton->BeginUpdate();
				
				upButton->Fill(Drawing::Color(0xFF6D6966));
				upButton->Clear(0, 0, 1, 1);
				upButton->Clear(12, 0, 1, 1);
				upButton->Clear(0, 19, 1, 1);
				upButton->Clear(12, 19, 1, 1);
				
				Drawing::Color border(0xFFBAB8B9);

				upButton->Fill(6, 7, 1, 1, border);
				upButton->Fill(5, 8, 3, 1, border);
				upButton->Fill(4, 9, 5, 1, border);
				upButton->Fill(3, 10, 7, 1, border);

				upButton->EndUpdate();
			}
			
			Drawing::ITexture *track = texture.Get(1);
			{
				Drawing::Size size = trackRect.GetSize();
			
				track->Create(size);
				track->BeginUpdate();
				
				track->FillGradient(Drawing::Color(0xFF51504B), Drawing::Color(0xFF262523));

				track->Clear(0, 0, 1, 1);
				track->Clear(size.Width - 1, 0, 1, 1);
				track->Clear(0, size.Height - 1, 1, 1);
				track->Clear(size.Width - 1, size.Height - 1, 1, 1);

				track->EndUpdate();
			}
			
			Drawing::ITexture *slider = texture.Get(2);
			{
				Drawing::Size size = sliderRect.GetSize();
			
				slider->Create(size);
				slider->BeginUpdate();
				
				slider->Fill(0xFF5F5E59);
				slider->Clear(0, 0, 1, 1);
				slider->Clear(size.Width - 1, 0, 1, 1);
				slider->Clear(0, size.Height - 1, 1, 1);
				slider->Clear(size.Width - 1, size.Height - 1, 1, 1);

				int temp = size.Height / 2 - 3;
				Drawing::Color point(0xFFE3E1E2);
				for (int i = 0; i < 8; i += 2)
				{
					for (int j = 0; j < 8; j += 2)
					{
						slider->Fill(3 + j, temp + i, 1, 1, point);
					}
				}

				slider->EndUpdate();
			}
			
			Drawing::ITexture *downButton = texture.Get(3);
			{
				downButton->Create(downButtonRect.GetSize());
				downButton->BeginUpdate();
				
				downButton->Fill(Drawing::Color(0xFF6D6966));
				downButton->Clear(0, 0, 1, 1);
				downButton->Clear(12, 0, 1, 1);
				downButton->Clear(0, 19, 1, 1);
				downButton->Clear(12, 19, 1, 1);
				
				Drawing::Color border(0xFFBAB8B9);

				downButton->Fill(6, 12, 1, 1, border);
				downButton->Fill(5, 11, 3, 1, border);
				downButton->Fill(4, 10, 5, 1, border);
				downButton->Fill(3, 9, 7, 1, border);

				downButton->EndUpdate();
			}
		}
		
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture.Get(0), upButtonRect.GetPosition());
		renderer->RenderTexture(texture.Get(1), trackRect.GetPosition());
		renderer->RenderTexture(texture.Get(2), sliderRect.GetPosition());
		renderer->RenderTexture(texture.Get(3), downButtonRect.GetPosition());
	}
	//---------------------------------------------------------------------------
}