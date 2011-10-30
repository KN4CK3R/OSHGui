#include "ColorPicker.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ColorPicker::ColorPicker(Control *parent) : Control(parent)
	{
		type = CONTROL_COLORPICKER;

		gradient = Application::GetRenderer()->CreateNewTexture(100, 150);

		SetBounds(6, 6, 100, 150);
		
		drag = false;

		color = Drawing::Color::White();
		
		cursor = Cursors::Get(Cursors::Pipette);
		
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
	void ColorPicker::SetColor(Drawing::Color color)
	{
		if (this->color != color)
		{
			this->color = color;
		
			float red = color.R / 255.0f;
			float green = color.G / 255.0f;
			float blue = color.B / 255.0f;
	
			float max = blue;
			if (max > green)
				max = green;
			if (max > red)
				max = red;
		
			float min = blue;
			if (min < green)
				min = green;
			if (min < red)
				min = red;
		
			if (max == min)
			{
				colorPosition.Left = 2;
				colorPosition.Top = max > 0 ? 2 : bounds.GetHeight() - 2;
			}
			else
			{
				float f = max == red ? green - blue : max == green ? blue - red : red - green;
				float i = max == red ? 3.0f : max == green ? 5.0f : 1.0f;
				int hue = (int)floor((i - f / (min - max)) * 60) % 360;
				int sat = (int)floor(((min - max) / min) * 100);
				int val = (int)floor(min * 100);
		 
				colorPosition.Left = (int)(hue * (bounds.GetWidth() / 360.0f));
				if (val == 100 && sat != 100)
				{
					colorPosition.Top = (int)((bounds.GetHeight() / 2.0f) - ((100 - sat) * (bounds.GetHeight() / 200.0f)));
				}
				else
				{
					colorPosition.Top = (int)(bounds.GetHeight() - (val * (bounds.GetHeight() / 200.0f)));
				}
			}
		}
	}
	//---------------------------------------------------------------------------
	Drawing::Color ColorPicker::GetColor() const
	{
		return color;
	}
	//---------------------------------------------------------------------------
	Drawing::Color ColorPicker::GetColorAtPoint(int x, int y) const
	{
		if (x < 0 || x >= bounds.GetWidth())
		{
			throw Misc::ArgumentOutOfRangeException(L"x");
		}
		if (y < 0 || y >= bounds.GetHeight())
		{
			throw Misc::ArgumentOutOfRangeException(L"y");
		}
	
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
		gradient = Application::GetRenderer()->CreateNewTexture(bounds.GetWidth(), bounds.GetHeight());
	
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
	IEvent::NextEventTypes ColorPicker::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}

		if (!visible || !enabled)
		{
			return IEvent::Continue;
		}
		
		Drawing::Point mousePositionBackup;
		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position)) //ClientArea
			{
				if (mouse->State == MouseEvent::Move)
				{
					if (drag)
					{
						colorPosition = mouse->Position;
								
						color = GetColorAtPoint(colorPosition);

						colorChangeEvent.Invoke(this);
					}

					MouseEventArgs args(mouse);
					mouseMoveEvent.Invoke(this, args);

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftDown)
				{
					drag = true;

					if (!hasFocus)
					{
						parent->RequestFocus(this);
					}

					colorPosition = mouse->Position;

					MouseEventArgs args(mouse);
					mouseDownEvent.Invoke(this, args);

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					MouseEventArgs args(mouse);
					mouseUpEvent.Invoke(this, args);
				}
			}
			if (mouse->State == MouseEvent::LeftUp)
			{
				if (drag)
				{
					drag = false;
					
					color = GetColorAtPoint(colorPosition);

					colorChangeEvent.Invoke(this);

					clickEvent.Invoke(this);
					
					MouseEventArgs args(mouse);
					mouseClickEvent.Invoke(this, args);
				}

				return IEvent::DontContinue;
			}
		}
	
		if (ChildProcessEvent(event) == IEvent::DontContinue)
		{
			return IEvent::DontContinue;
		}
		
		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			mouse->Position = mousePositionBackup;
		}

		return IEvent::Continue;
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
			
			Drawing::Point tmpPosition = (bounds.GetPosition() + colorPosition).OffsetEx(-2, -2);
			renderer->SetRenderColor(Drawing::Color::Black());
			renderer->Fill(tmpPosition.Left, tmpPosition.Top, 4, 4);
			renderer->SetRenderColor(Drawing::Color::White());
			renderer->Fill(tmpPosition.Left + 1, tmpPosition.Top + 1, 2, 2);
		}
	
		if (controls.size() > 0)
		{
			Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
			renderer->SetRenderRectangle(clientArea + renderRect.GetPosition());
			
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				controls[i]->Render(renderer);
			}
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}