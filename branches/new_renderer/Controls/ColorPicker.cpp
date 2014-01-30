/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "ColorPicker.hpp"
#include "../Drawing/CustomizableImage.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeI ColorPicker::DefaultSize(100, 150);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ColorPicker::ColorPicker()
		: drag(false),
		  color(Drawing::Color::White())
	{
		type = ControlType::ColorPicker;
		
		cursor = Cursors::Get(Cursors::Pipette);

		ApplyTheme(Application::Instance().GetTheme());

		SetSize(DefaultSize);
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ColorPicker::SetSize(const Drawing::SizeI &size)
	{
		if (this->size != size)
		{
			Control::SetSize(size);

			CreateGradientTexture();
		}
	}
	//---------------------------------------------------------------------------
	void ColorPicker::SetColor(const Drawing::Color &color_)
	{
		if (color != color_)
		{
			color = color_;
		
			CalculateColorCursorLocation();

			colorChangedEvent.Invoke(this, color);

			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	const Drawing::Color& ColorPicker::GetColor() const
	{
		return color;
	}
	//---------------------------------------------------------------------------
	Drawing::Color ColorPicker::GetColorAtPoint(const Drawing::PointI &point) const
	{
		using namespace Drawing;

		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (point.X < 0 || point.X >= GetWidth())
		{
			throw Misc::ArgumentOutOfRangeException("x");
		}
		if (point.Y < 0 || point.Y >= GetHeight())
		{
			throw Misc::ArgumentOutOfRangeException("y");
		}
		#endif

		double hue = (1.0 / GetWidth()) * point.X;
		hue = hue - (int)hue;
		double saturation, brightness;
		if (point.Y <= GetHeight() / 2.0)
		{
			saturation = point.Y / (GetHeight() / 2.0);
			brightness = 1;
		}
		else
		{
			saturation = (GetHeight() / 2.0) / point.Y;
			brightness = ((GetHeight() / 2.0) - point.Y + (GetHeight() / 2.0)) / point.Y;
		}

		return Color::FromHSB(hue, saturation, brightness);
	}
	//---------------------------------------------------------------------------
	ColorChangedEvent& ColorPicker::GetColorChangedEvent()
	{
		return colorChangedEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void ColorPicker::CreateGradientTexture()
	{
		Drawing::CustomizableImage image(GetSize());
		for (int y = 0; y < GetHeight(); ++y)
		{
			for (int x = 0; x < GetWidth(); ++x)
			{
				image.SetRectangle(Drawing::RectangleI(x, y, 1, 1), GetColorAtPoint(x, y));
			}
		}

		gradient = Drawing::Image::FromCustomizableImage(image);
	}
	//---------------------------------------------------------------------------
	void ColorPicker::CalculateColorCursorLocation()
	{
		float red = color.GetRed();
		float green = color.GetGreen();
		float blue = color.GetBlue();
	
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
			int hue = (int)std::floor((i - f / (min - max)) * 60) % 360;
			int sat = (int)std::floor(((min - max) / min) * 100);
			int val = (int)std::floor(min * 100);
		 
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
	void ColorPicker::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry);

		g.DrawImage(gradient, Color::White(), RectangleF(PointF(0, 0), GetSize()));

		g.FillRectangle(Color::Black(), RectangleF(colorCursorLocation - PointF(2, 2), SizeF(4, 4)));
		g.FillRectangle(Color::White(), RectangleF(colorCursorLocation - PointF(1, 1), SizeF(2, 2)));
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
			
			colorChangedEvent.Invoke(this, color);

			Invalidate();
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
			
			colorChangedEvent.Invoke(this, color);

			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
}