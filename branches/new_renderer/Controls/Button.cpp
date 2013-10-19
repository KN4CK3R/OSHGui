/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Button.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeF Button::DefaultSize(92, 24);
	const Drawing::PointF Button::DefaultLabelOffset(6, 5);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Button::Button()
		: label(new Label())
	{
		type = ControlType::Button;

		label->SetLocation(DefaultLabelOffset);
		label->SetBackColor(Drawing::Color::Empty());

		SetSize(DefaultSize);
		
		ApplyTheme(Application::Instance()->GetTheme());
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Button::SetAutoSize(bool autoSize)
	{
		Control::SetAutoSize(autoSize);

		label->SetAutoSize(autoSize);
	}
	//---------------------------------------------------------------------------
	void Button::SetSize(const Drawing::SizeF &size)
	{
		Control::SetSize(size);

		CalculateLabelLocation();
	}
	//---------------------------------------------------------------------------
	void Button::SetText(const Misc::AnsiString &text)
	{
		label->SetText(text);
		if (autoSize)
		{
			SetSize(label->GetSize().InflateEx(DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2));
		}
		CalculateLabelLocation();
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& Button::GetText() const
	{
		return label->GetText();
	}
	//---------------------------------------------------------------------------
	void Button::SetFont(const Drawing::FontPtr &font)
	{
		Control::SetFont(font);

		label->SetFont(font);
		if (autoSize)
		{
			SetSize(label->GetSize().InflateEx(DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2));
		}
	}
	//---------------------------------------------------------------------------
	void Button::SetForeColor(Drawing::Color color)
	{
		Control::SetForeColor(color);

		label->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Button::Intersect(const Drawing::PointF &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void Button::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();

		label->SetParent(this);
	}
	//---------------------------------------------------------------------------
	void Button::CalculateLabelLocation()
	{
		label->SetLocation(Drawing::PointF(GetSize().Width / 2 - label->GetSize().Width / 2, GetSize().Height / 2 - label->GetSize().Height / 2));
	}
	//---------------------------------------------------------------------------
	void Button::DrawSelf(Drawing::RenderContext &context)
	{
		Control::DrawSelf(context);

		label->Render();
	}
	//---------------------------------------------------------------------------
	void Button::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(geometry);

		auto tempColor = backColor;
		if ((isFocused || isInside) && !(isFocused && isClicked))
		{
			tempColor += mouseOverFocusColor;
		}

		auto color = tempColor + Color(0, 10, 10, 10);
		g.FillRectangle(color, RectangleF(PointF(1, 0), GetSize() - SizeF(2, 1)));
		g.FillRectangle(color, RectangleF(PointF(0, 1), GetSize() - SizeF(0, 3)));
		color = tempColor - Color(0, 50, 50, 50);
		g.FillRectangle(color, RectangleF(PointF(1, GetHeight() - 2), SizeF(GetWidth() - 2, 2)));
		g.FillRectangle(color, RectangleF(PointF(GetWidth() - 1, 1), SizeF(1, GetHeight() - 2)));

		ColorRectangle colors(tempColor, tempColor, GetBackColor() - Color(0, 20, 20, 20), GetBackColor() - Color(0, 20, 20, 20));
		g.FillRectangleGradient(colors, RectangleF(PointF(1, 2), GetSize() - SizeF(2, 4)));
		g.FillRectangleGradient(colors, RectangleF(PointF(2, 1), GetSize() - SizeF(4, 2)));
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	bool Button::OnKeyUp(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyUp(keyboard))
		{
			if (keyboard.GetKeyCode() == Key::Return)
			{
				clickEvent.Invoke(this);
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
}
