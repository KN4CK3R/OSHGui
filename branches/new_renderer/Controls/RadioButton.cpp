/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "RadioButton.hpp"
#include "Label.hpp"
#include "../Misc/Exceptions.hpp"
#include "ContainerControl.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	RadioButton::RadioButton()
	{
		type = CONTROL_RADIOBUTTON;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void RadioButton::SetChecked(bool checked)
	{
		if (this->checked != checked)
		{
			if (GetParent() != nullptr)
			{
				//uncheck other radiobuttons
				auto controls = static_cast<ContainerControl*>(GetParent())->GetControls();
				for (auto it = std::begin(controls); it != std::end(controls); ++it)
				{
					Control *control = *it;
					if (control->GetType() == CONTROL_RADIOBUTTON)
					{
						RadioButton *radio = static_cast<RadioButton*>(control);
						radio->SetCheckedInternal(false);
					}
				}
			
				SetCheckedInternal(checked);
			}
		}
	}
	//---------------------------------------------------------------------------
	void RadioButton::SetCheckedInternal(bool checked)
	{
		if (this->checked != checked)
		{
			this->checked = checked;
			
			checkedChangedEvent.Invoke(this);

			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	void RadioButton::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(geometry);

		g.FillRectangle(GetBackColor(), RectangleF(PointF(0, 0), SizeF(DefaultCheckBoxSize, DefaultCheckBoxSize)));

		g.FillRectangleGradient(ColorRectangle(Color::White(), Color::White() - Color(0, 137, 137, 137)), RectangleF(PointF(1, 1), SizeF(15, 15)));
		g.FillRectangleGradient(ColorRectangle(GetBackColor(), GetBackColor() + Color(0, 55, 55, 55)), RectangleF(PointF(2, 2), SizeF(13, 13)));

		if (checked)
		{
			g.FillRectangle(Color::White() - Color(0, 128, 128, 128), RectangleF(PointF(5, 7), SizeF(7, 3)));
			ColorRectangle colors(Color::White(), Color::White() - Drawing::Color(0, 137, 137, 137));
			g.FillRectangleGradient(colors, RectangleF(PointF(7, 5), SizeF(3, 7)));
			g.FillRectangleGradient(colors, RectangleF(PointF(6, 6), SizeF(5, 5)));
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void RadioButton::OnMouseClick(const MouseMessage &mouse)
	{
		Control::OnMouseClick(mouse);

		SetChecked(true);
	}
	//---------------------------------------------------------------------------
	bool RadioButton::OnKeyUp(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyUp(keyboard))
		{
			if (keyboard.GetKeyCode() == Key::Space)
			{
				SetChecked(true);

				clickEvent.Invoke(this);
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
	
}