#include "ColorBar.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ColorBar::ColorBar(Control *parent) : Control(parent)
	{
		type = CONTROL_COLORBAR;
		
		for (int i = 0; i < 3; ++i)
		{
			bars.push_back(Application::Renderer->CreateNewTexture());
			barSliders.push_back(Drawing::Point());
			drag[i] = false;
		}
		
		color = Drawing::Color::White();
		
		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	ColorBar::~ColorBar()
	{
	
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ColorBar::SetColor(Drawing::Color color)
	{
		if (this->color != color)
		{
			this->color = color;

			UpdateBars();

			colorChangeEvent.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
	Drawing::Color ColorBar::GetColor() const
	{
		return color;
	}
	//---------------------------------------------------------------------------
	ColorChangeEvent& ColorBar::GetColorChangeEvent()
	{
		return colorChangeEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ColorBar::CanHaveFocus() const
	{
		return enabled && visible;
	}
	//---------------------------------------------------------------------------
	bool ColorBar::ContainsPoint(const Drawing::Point &point) const
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void ColorBar::Invalidate()
	{
		if (bounds.GetHeight() != 45)
		{
			bounds.SetHeight(45);
		}
	
		clientArea = bounds.OffsetEx(4, 0).InflateEx(-8, 0);
		
		UpdateBars();

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	void ColorBar::CreateBarTexture(int index)
	{
		if (index < 0 || index > 2)
		{
			return;
		}
	
		std::shared_ptr<Drawing::ITexture> bar = bars[index];
		
		int width = clientArea.GetWidth();
		
		bar->Create(width, 10);
	
		width -= 2;
		float multi = 255.0f / width;
		
		bar->BeginUpdate();
		bar->Fill(foreColor);
		for(int x = 0; x < width; ++x)
		{
			switch (index)
			{
				case 0:
					bar->Fill(x + 1, 1, 1, 8, Drawing::Color(x * multi, this->color.G, this->color.B));
					break;
				case 1:
					bar->Fill(x + 1, 1, 1, 8, Drawing::Color(this->color.R, x * multi, this->color.B));
					break;
				case 2:
					bar->Fill(x + 1, 1, 1, 8, Drawing::Color(this->color.R, this->color.G, x * multi));
					break;
			}
		}
		bar->EndUpdate();
	}
	//---------------------------------------------------------------------------
	void ColorBar::UpdateBars()
	{
		for (int i = 0; i < 3; ++i)
		{
			CreateBarTexture(i);
			
			float multi = (clientArea.GetWidth() - 2) / 255.0f;				
			barSliders[i].Left = (i == 0 ? color.R : i == 1 ? color.G : color.B) * multi;
			barSliders[i].Top = i * 15 + 9;				
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes ColorBar::ProcessEvent(Event *event)
	{
		if (event == 0)
		{
			return Event::DontContinue;
		}

		if (!visible || !enabled)
		{
			return Event::Continue;
		}
		
		Drawing::Point mousePositionBackup;
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			for (int i = 0; i < 3; ++i)
			{
				if (drag[i] == true)
				{
					if (mouse->State == MouseEvent::Move)
					{
						barSliders[i].Left = mouse->Position.Left < 0 ? 0 : mouse->Position.Left > clientArea.GetWidth() - 2 ? clientArea.GetWidth() - 2 : mouse->Position.Left;
						barSliders[i].Top = i * 15 + 11;
					
						float multi = 255.0f / (clientArea.GetWidth() - 2);
						
						(i == 0 ? color.R : i == 1 ? color.G : color.B) = multi * barSliders[i].Left;

						UpdateBars();

						colorChangeEvent.Invoke(this);

						mouseMoveEvent.Invoke(this, MouseEventArgs(mouse));

						return Event::DontContinue;
					}
					else if (mouse->State == MouseEvent::LeftUp)
					{
						drag[i] = false;

						barSliders[i].Left = mouse->Position.Left < 0 ? 0 : mouse->Position.Left > clientArea.GetWidth() - 2 ? clientArea.GetWidth() - 2 : mouse->Position.Left;
						barSliders[i].Top = i * 15 + 11;
					
						float multi = 255.0f / (clientArea.GetWidth() - 2);
						
						(i == 0 ? color.R : i == 1 ? color.G : color.B) = multi * barSliders[i].Left;

						UpdateBars();

						colorChangeEvent.Invoke(this);

						clickEvent.Invoke(this);

						mouseClickEvent.Invoke(this, MouseEventArgs(mouse));

						mouseUpEvent.Invoke(this, MouseEventArgs(mouse));
						
						return Event::DontContinue;
					}
				}
				else if (Drawing::Rectangle(0, i * 15, clientArea.GetWidth(), 12).Contains(mouse->Position))
				{
					if (mouse->State == MouseEvent::LeftDown)
					{
						drag[i] = true;

						if (!hasFocus)
						{
							Parent->RequestFocus(this);
						}

						mouseDownEvent.Invoke(this, MouseEventArgs(mouse));

						return Event::DontContinue;
					}
				}
			}
		}
	
		if (ProcessChildrenEvent(event) == Event::DontContinue)
		{
			return Event::DontContinue;
		}
		
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			mouse->Position = mousePositionBackup;
		}

		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void ColorBar::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
		
		for (int i = 0; i < 3; ++i)
		{
			renderer->SetRenderColor(Drawing::Color::White());
			renderer->RenderTexture(bars[i], clientArea.GetLeft(), clientArea.GetTop() + i * 15, clientArea.GetWidth(), 8);
			
			renderer->SetRenderColor(foreColor);
			Drawing::Point sliderPos = barSliders[i].OffsetEx(clientArea.GetLeft() + 1, clientArea.GetTop());
			for (int j = 0; j < 3; ++j)
			{
				renderer->Fill(sliderPos.Left - j, sliderPos.Top + j, 1 + j * 2, 1);
			}
		}
	
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