/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "ColorPicker.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::Size ColorPicker::DefaultSize(100, 150);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ColorPicker::ColorPicker()
		: drag(false),
		  color(Drawing::Color::White())
	{
		type = CONTROL_COLORPICKER;
		
		cursor = Cursors::Get(Cursors::Pipette);

		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color::Empty());

		SetSize(DefaultSize);
	}
	//---------------------------------------------------------------------------
	ColorPicker::~ColorPicker()
	{
	
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ColorPicker::SetSize(const Drawing::Size &size)
	{
		if (this->size != size)
		{
			Control::SetSize(size);

			CreateGradientTexture();
		}
	}
	//---------------------------------------------------------------------------
	void ColorPicker::SetColor(const Drawing::Color &color)
	{
		if (this->color != color)
		{
			this->color = color;
		
			CalculateColorCursorLocation();

			Drawing::Color args = color;
			colorChangedEvent.Invoke(this, args);
		}
	}
	//---------------------------------------------------------------------------
	const Drawing::Color& ColorPicker::GetColor() const
	{
		return color;
	}
	//---------------------------------------------------------------------------
	const Drawing::Color ColorPicker::GetColorAtPoint(int x, int y) const
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (x < 0 || x >= GetWidth())
		{
			throw Misc::ArgumentOutOfRangeException("x");
		}
		if (y < 0 || y >= GetHeight())
		{
			throw Misc::ArgumentOutOfRangeException("y");
		}
		#endif
	
		Drawing::Color tmpColor;
		
		double hue = (1.0 / GetWidth()) * x;
		hue = hue - (int)hue;
		double saturation, brightness;
		if (y <= GetHeight() / 2.0)
		{
			saturation = y / (GetHeight() / 2.0);
			brightness = 1;
		}
		else
		{
			saturation = (GetHeight() / 2.0) / y;
			brightness = ((GetHeight() / 2.0) - y + (GetHeight() / 2.0)) / y;
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
	const Drawing::Color ColorPicker::GetColorAtPoint(const Drawing::Point &point) const
	{
		return GetColorAtPoint(point.X, point.Y);
	}
	//---------------------------------------------------------------------------
	ColorChangedEvent& ColorPicker::GetColorChangedEvent()
	{
		return colorChangedEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ColorPicker::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void ColorPicker::CreateGradientTexture()
	{
		gradient = Application::Instance()->GetRenderer()->CreateNewTexture(size);
	
		gradient->BeginUpdate();
		for (int y = 0; y < GetHeight(); ++y)
		{
			for(int x = 0; x < GetWidth(); ++x)
			{
				gradient->Fill(x, y, GetColorAtPoint(x, y));
			}
		}
		gradient->EndUpdate();
	}
	//---------------------------------------------------------------------------
	void ColorPicker::CalculateColorCursorLocation()
	{
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
			colorCursorLocation.Left = 2;
			colorCursorLocation.Top = max > 0 ? 2 : GetHeight() - 2;
		}
		else
		{
			float f = max == red ? green - blue : max == green ? blue - red : red - green;
			float i = max == red ? 3.0f : max == green ? 5.0f : 1.0f;
			int hue = (int)floor((i - f / (min - max)) * 60) % 360;
			int sat = (int)floor(((min - max) / min) * 100);
			int val = (int)floor(min * 100);
		 
			colorCursorLocation.Left = (int)(hue * (GetWidth() / 360.0f));
			if (val == 100 && sat != 100)
			{
				colorCursorLocation.Top = (int)((GetHeight() / 2.0f) - ((100 - sat) * (GetHeight() / 200.0f)));
			}
			else
			{
				colorCursorLocation.Top = (int)(GetHeight() - (val * (GetHeight() / 200.0f)));
			}
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void ColorPicker::OnMouseDown(const MouseMessage &mouse)
	{
		Control::OnMouseDown(mouse);

		drag = true;
	}
	//---------------------------------------------------------------------------
	void ColorPicker::OnMouseMove(const MouseMessage &mouse)
	{
		Control::OnMouseMove(mouse);
		
		if (drag)
		{
			colorCursorLocation = mouse.Location - absoluteLocation;
			
			color = GetColorAtPoint(colorCursorLocation);
			Drawing::Color args = color;
			colorChangedEvent.Invoke(this, args);
		}
	}
	//---------------------------------------------------------------------------
	void ColorPicker::OnMouseUp(const MouseMessage &mouse)
	{
		Control::OnMouseUp(mouse);

		if (drag)
		{
			drag = false;

			colorCursorLocation = mouse.Location - absoluteLocation;
					
			color = GetColorAtPoint(colorCursorLocation);
			Drawing::Color colorArgs = color;
			colorChangedEvent.Invoke(this, colorArgs);
		}
	}
	//---------------------------------------------------------------------------
	void ColorPicker::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		if (gradient != 0)
		{
			renderer->SetRenderColor(Drawing::Color::White());
			renderer->RenderTexture(gradient, absoluteLocation);
			
			Drawing::Point tmpLocation = (absoluteLocation + colorCursorLocation).OffsetEx(-2, -2);
			renderer->SetRenderColor(Drawing::Color::Black());
			renderer->Fill(tmpLocation.Left, tmpLocation.Top, 4, 4);
			renderer->SetRenderColor(Drawing::Color::White());
			renderer->Fill(tmpLocation.Left + 1, tmpLocation.Top + 1, 2, 2);
		}
	}
	//---------------------------------------------------------------------------
}