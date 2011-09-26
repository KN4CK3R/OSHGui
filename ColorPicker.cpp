#include "ColorPicker.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ColorPicker::ColorPicker(Control *parent) : Control(parent)
	{
		type = CONTROL_COLORPICKER;
		
		gradient = Application::Renderer->CreateNewTexture();
		drag = false;
		
		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color::Empty());
	}
	//---------------------------------------------------------------------------
	ColorPicker::~ColorPicker()
	{
	
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	Drawing::Color ColorPicker::GetColor() const
	{
		return color;
	}
	//---------------------------------------------------------------------------
	Drawing::Color ColorPicker::GetColorAtPoint(int x, int y) const
	{
		Drawing::Color tmpColor;
		
		double hue = (1.0 / bounds.GetWidth()) * x;
		hue = hue - (int)hue;
		double saturation, brightness;
		if (y <= (bounds.GetHeight() / 2))
		{
			saturation = (float)y / (bounds.GetHeight() / 2);
			brightness = 1;
		}
		else
		{
			saturation = (float)(bounds.GetHeight() / 2) / y;
			brightness = ((float)(bounds.GetHeight() / 2) - y + ((bounds.GetHeight() / 2))) / y;
		}
		
		double h = hue == 1.0 ? 0 : hue * 6.0;
		double f = h - (int)h;
		double p = brightness * (1.0 - saturation);
		double q = brightness * (1.0 - saturation * f);
		double t = brightness * (1.0 - (saturation * (1.0 - f)));
		if (h < 1)
		{
			tmpColor = Drawing::Color(
							(int)(brightness * 255),
							(int)(t * 255),
							(int)(p * 255)
							);
		}
		else if (h < 2)
		{
			tmpColor = Drawing::Color(
							(int)(q * 255),
							(int)(brightness * 255),
							(int)(p * 255)
							);
		}
		else if (h < 3)
		{
			tmpColor = Drawing::Color(
							(int)(p * 255),
							(int)(brightness * 255),
							(int)(t * 255)
							);
		}
		else if (h < 4)
		{
			tmpColor = Drawing::Color(
							(int)(p * 255),
							(int)(q * 255),
							(int)(brightness * 255)
							);
		}
		else if (h < 5)
		{
			tmpColor = Drawing::Color(
							(int)(t * 255),
							(int)(p * 255),
							(int)(brightness * 255)
							);
		}
		else
		{
			tmpColor = Drawing::Color(
							(int)(brightness * 255),
							(int)(p * 255),
							(int)(q * 255)
							);
		}
		
		return tmpColor;
	}
	//---------------------------------------------------------------------------
	Drawing::Color ColorPicker::GetColorAtPoint(const Drawing::Point &point) const
	{
		return GetColorAtPoint(point.X, point.Y);
	}
	//---------------------------------------------------------------------------
	ColorChangeEvent& ColorPicker::GetColorChangeEvent()
	{
		return colorChangeEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ColorPicker::CanHaveFocus() const
	{
		return enabled && visible;
	}
	//---------------------------------------------------------------------------
	bool ColorPicker::ContainsPoint(const Drawing::Point &point) const
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void ColorPicker::Invalidate()
	{
		if (clientArea != bounds)
		{
			clientArea = bounds;
			
			CreateGradientTexture();
		}
		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	void ColorPicker::CreateGradientTexture()
	{
		gradient->Create(bounds.GetWidth(), bounds.GetHeight());
	
		gradient->BeginUpdate();
		for (int y = 0; y < bounds.GetHeight(); ++y)
		{
			for(int x = 0; x < bounds.GetWidth(); ++x)
			{
				gradient->Fill(x, y, GetColorAtPoint(x, y));
			}
		}
		gradient->EndUpdate();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes ColorPicker::ProcessEvent(Event *event)
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

			if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position) || drag) //ClientArea
			{
				if (mouse->State == MouseEvent::Move && drag == true)
				{
					color = GetColorAtPoint(mouse->Position);

					colorChangeEvent.Invoke(this);

					mouseMoveEvent.Invoke(this, MouseEventArgs(mouse));

					return Event::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftDown)
				{
					drag = true;

					if (!hasFocus)
					{
						Parent->RequestFocus(this);
					}

					mouseDownEvent.Invoke(this, MouseEventArgs(mouse));

					return Event::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					drag = false;

					color = GetColorAtPoint(mouse->Position);

					colorChangeEvent.Invoke(this);

					clickEvent.Invoke(this);
					
					mouseClickEvent.Invoke(this, MouseEventArgs(mouse));

					mouseUpEvent.Invoke(this, MouseEventArgs(mouse));

					return Event::DontContinue;
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
	void ColorPicker::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
		
		if (gradient != 0)
		{
			renderer->SetRenderColor(Drawing::Color::White());
			renderer->RenderTexture(gradient, bounds.GetPosition());
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