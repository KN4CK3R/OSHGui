#include "ColorBar.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ColorBar::ColorBar(Control *parent) : Control()
	{
		type = CONTROL_COLORBAR;
		
		barIndex = 0;
		
		for (int i = 0; i < 3; ++i)
		{
			bars.push_back(Application::Instance()->GetRenderer()->CreateNewTexture(150, 10));
			barSliders.push_back(Drawing::Point());
			drag[i] = false;
		}
		
		color = Drawing::Color::White();
		
		SetBounds(6, 6, 150, 45);
		
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
			Drawing::Color args = color;
			colorChangedEvent.Invoke(this, args);
		}
	}
	//---------------------------------------------------------------------------
	Drawing::Color ColorBar::GetColor() const
	{
		return color;
	}
	//---------------------------------------------------------------------------
	ColorChangedEvent& ColorBar::GetColorChangedEvent()
	{
		return colorChangedEvent;
	}
	//---------------------------------------------------------------------------
	KeyDownEvent& ColorBar::GetKeyDownEvent()
	{
		return keyDownEvent;
	}
	//---------------------------------------------------------------------------
	KeyUpEvent& ColorBar::GetKeyUpEvent()
	{
		return keyUpEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ColorBar::CanHaveFocus() const
	{
		return isEnabled && isVisible;
	}
	//---------------------------------------------------------------------------
	bool ColorBar::ContainsPoint(const Drawing::Point &point) const
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	const Drawing::Point ColorBar::PointToClient(const Drawing::Point &point) const
	{
		return Drawing::Point(point.Left - clientArea.GetLeft(), point.Top - clientArea.GetTop());
	}
	//---------------------------------------------------------------------------
	void ColorBar::Invalidate()
	{
		if (bounds.GetHeight() != 45)
		{
			bounds.SetHeight(45);
		}
	
		Drawing::Rectangle newClientArea = bounds.OffsetEx(4, 0).InflateEx(-8, 0);
		if (newClientArea != clientArea)
		{
			clientArea = newClientArea;

			UpdateBars();
		}

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	void ColorBar::CreateBarTexture(int index)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (index < 0 || index > 2)
		{
			throw Misc::ArgumentOutOfRangeException("index", __FILE__, __LINE__);
		}
		#endif
	
		std::shared_ptr<Drawing::ITexture> bar = bars[index];
		
		int width = clientArea.GetWidth();
		
		if (width != bar->GetSize().Width)
		{
			bar = Application::Instance()->GetRenderer()->CreateNewTexture(width, 10);
			bars[index] = bar;
		}
	
		width -= 2;
		float multi = 255.0f / width;
		
		bar->BeginUpdate();
		bar->Fill(foreColor);
		for(int x = 0; x < width; ++x)
		{
			switch (index)
			{
				case 0:
					//bar->FillGradient(1, 1, width, 8, Drawing::Color(0, color.G, color.B), Drawing::Color(255, color.G, color.B), false);
					bar->Fill(x + 1, 1, 1, 8, Drawing::Color((unsigned int)(x * multi), this->color.G, this->color.B));
					break;
				case 1:
					//bar->FillGradient(1, 1, width, 8, Drawing::Color(color.R, 0, color.B), Drawing::Color(color.R, 255, color.B), false);
					bar->Fill(x + 1, 1, 1, 8, Drawing::Color(this->color.R, (unsigned int)(x * multi), this->color.B));
					break;
				case 2:
					//bar->FillGradient(1, 1, width, 8, Drawing::Color(color.R, color.G, 0), Drawing::Color(color.R, color.G, 255), false);
					bar->Fill(x + 1, 1, 1, 8, Drawing::Color(this->color.R, this->color.G, (unsigned int)(x * multi)));
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
			barSliders[i].Left = (int)((i == 0 ? color.R : i == 1 ? color.G : color.B) * multi + 0.5f);
			barSliders[i].Top = i * 15 + 9;
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	bool ColorBar::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException("event", __FILE__, __LINE__);
		}

		if (!isVisible || !isEnabled)
		{
			return false;
		}
		
		Drawing::Point mousePositionBackup;
		if (event->Type == IEvent::Mouse)
		{
			MouseMessage *mouse = (MouseMessage*)event;
			mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
		}
	
		if (ChildProcessEvent(event) == true)
		{
			return true;
		}
		
		if (event->Type == IEvent::Mouse)
		{
			MouseMessage *mouse = (MouseMessage*)event;
			
			for (int i = 0; i < 3; ++i)
			{
				if (drag[i] == true)
				{
					if (mouse->State == MouseMessage::Move)
					{
						barSliders[i].Left = mouse->Position.Left < 0 ? 0 : mouse->Position.Left > clientArea.GetWidth() - 2 ? clientArea.GetWidth() - 2 : mouse->Position.Left;
						barSliders[i].Top = i * 15 + 11;
					
						float multi = 255.0f / (clientArea.GetWidth() - 2);
						
						(i == 0 ? color.R : i == 1 ? color.G : color.B) =  (unsigned char)(multi * barSliders[i].Left + 0.5f);

						UpdateBars();
						Drawing::Color colorArgs = color;
						colorChangedEvent.Invoke(this, colorArgs);

						MouseEventArgs args(mouse);
						mouseMoveEvent.Invoke(this, args);

						return true;
					}
					else if (mouse->State == MouseMessage::LeftUp)
					{
						drag[i] = false;

						barSliders[i].Left = mouse->Position.Left < 0 ? 0 : mouse->Position.Left > clientArea.GetWidth() - 2 ? clientArea.GetWidth() - 2 : mouse->Position.Left;
						barSliders[i].Top = i * 15 + 11;
					
						float multi = 255.0f / (clientArea.GetWidth() - 2);
						
						(i == 0 ? color.R : i == 1 ? color.G : color.B) =  (unsigned char)(multi * barSliders[i].Left + 0.5f);

						UpdateBars();
						Drawing::Color colorArgs = color;
						colorChangedEvent.Invoke(this, colorArgs);

						clickEvent.Invoke(this);

						MouseEventArgs args(mouse);
						mouseClickEvent.Invoke(this, args);

						args = MouseEventArgs(mouse);
						mouseUpEvent.Invoke(this, args);
						
						return true;
					}
				}
				else if (Drawing::Rectangle(0, i * 15, clientArea.GetWidth(), 12).Contains(mouse->Position))
				{
					if (mouse->State == MouseMessage::LeftDown)
					{
						barIndex = i;
					
						drag[i] = true;

						if (!isFocused)
						{
							parent->RequestFocus(this);
						}

						MouseEventArgs args(mouse);
						mouseDownEvent.Invoke(this, args);

						return true;
					}
				}
			}

			if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position))
			{
				return true;
			}

			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == IEvent::Keyboard)
		{
			KeyboardMessage *keyboard = (KeyboardMessage*)event;
			if (keyboard->State == KeyboardMessage::KeyDown)
			{
				KeyEventArgs args(keyboard);
				keyDownEvent.Invoke(this, args);
				if (!args.Handled)
				{
					if (keyboard->KeyCode == Key::Left || keyboard->KeyCode == Key::Right)
					{
						barSliders[barIndex].Left += keyboard->KeyCode == Key::Left ? -1 : 1;
						
						if (barSliders[barIndex].Left < 0)
						{
							barSliders[barIndex].Left = 0;
						}
						if (barSliders[barIndex].Left >= clientArea.GetWidth() - 2)
						{
							barSliders[barIndex].Left = clientArea.GetWidth() - 2;
						}
					
						float multi = 255.0f / (clientArea.GetWidth() - 2);
						
						(barIndex == 0 ? color.R : barIndex == 1 ? color.G : color.B) = (unsigned char)(multi * barSliders[barIndex].Left + 0.5f);

						UpdateBars();
						Drawing::Color args = color;
						colorChangedEvent.Invoke(this, args);
					}
				}
			}
			else if (keyboard->State == KeyboardMessage::KeyUp)
			{
				KeyEventArgs args(keyboard);
				keyUpEvent.Invoke(this, args);
			}
			
			return true;
		}

		return false;
	}
	//---------------------------------------------------------------------------
	void ColorBar::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
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
			
			ChildRender(renderer);
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}