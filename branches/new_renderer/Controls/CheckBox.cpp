/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "CheckBox.hpp"
#include "Label.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeF CheckBox::DefaultLabelOffset(20, 2);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	CheckBox::CheckBox()
		: checked(false),
		  label(new Label())
	{
		type = CONTROL_CHECKBOX;
		
		SetSize(DefaultCheckBoxSize, DefaultCheckBoxSize);
		SetAutoSize(true);

		label->SetLocation(Drawing::PointF(DefaultLabelOffset.Width, DefaultLabelOffset.Height));
		label->SetBackColor(Drawing::Color::Empty());

		ApplyTheme(Application::Instance()->GetTheme());
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void CheckBox::SetChecked(bool checked)
	{
		if (this->checked != checked)
		{
			this->checked = checked;
			
			checkedChangedEvent.Invoke(this);

			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	bool CheckBox::GetChecked() const
	{
		return checked;
	}
	//---------------------------------------------------------------------------
	void CheckBox::SetText(const Misc::AnsiString &text)
	{
		Drawing::PointF offset = label->GetLocation();

		label->SetText(text);
		if (autoSize)
		{
			size = label->GetSize().InflateEx(offset.Left, offset.Top);
			if (size.Height < DefaultCheckBoxSize)
			{
				size.Height = DefaultCheckBoxSize;
			}
		}
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& CheckBox::GetText() const
	{
		return label->GetText();
	}
	//---------------------------------------------------------------------------
	void CheckBox::SetFont(const std::shared_ptr<Drawing::IFont> &font)
	{
		Control::SetFont(font);

		label->SetFont(font);
		if (autoSize)
		{
			size = label->GetSize();
			if (font->GetSize() < DefaultCheckBoxSize)
			{
				checkBoxLocation = Drawing::PointF(0, 0);
				int y = (int)(DefaultCheckBoxSize / 2.0f - font->GetSize() / 2.0f + 0.5f);
				label->SetLocation(Drawing::PointF(DefaultLabelOffset.Width, y));
			}
			else
			{
				label->SetLocation(Drawing::PointF(DefaultLabelOffset.Width, DefaultLabelOffset.Height));
				int y = (int)(font->GetSize() / 2.0f - DefaultCheckBoxSize / 2.0f + 0.5f);
				checkBoxLocation = Drawing::PointF(0, y);
			}
		}
	}
	//---------------------------------------------------------------------------
	void CheckBox::SetForeColor(Drawing::Color color)
	{
		Control::SetForeColor(color);

		label->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	CheckedChangedEvent& CheckBox::GetCheckedChangedEvent()
	{
		return checkedChangedEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool CheckBox::Intersect(const Drawing::PointF &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void CheckBox::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();

		checkBoxAbsoluteLocation = absoluteLocation + checkBoxLocation;

		label->SetParent(this);
	}
	//---------------------------------------------------------------------------
	void CheckBox::DrawSelf(Drawing::RenderContext &context)
	{
		Control::DrawSelf(context);

		label->Render_();
	}
	//---------------------------------------------------------------------------
	void CheckBox::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		renderer->SetRenderColor(backColor);
		renderer->Fill(checkBoxAbsoluteLocation.Left, checkBoxAbsoluteLocation.Top, DefaultCheckBoxSize, DefaultCheckBoxSize);

		Drawing::Color white = Drawing::Color::White();
		renderer->SetRenderColor(white);
		renderer->FillGradient(checkBoxAbsoluteLocation.Left + 1, checkBoxAbsoluteLocation.Top + 1, 15, 15, white - Drawing::Color(0, 137, 137, 137));
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(checkBoxAbsoluteLocation.Left + 2, checkBoxAbsoluteLocation.Top + 2, 13, 13, backColor + Drawing::Color(0, 55, 55, 55));

		renderer->SetRenderColor(white);
		if (checked)
		{
			renderer->Fill(checkBoxAbsoluteLocation.Left + 5, checkBoxAbsoluteLocation.Top + 5, 7, 7);
			renderer->FillGradient(checkBoxAbsoluteLocation.Left + 6, checkBoxAbsoluteLocation.Top + 6, 5, 5, white - Drawing::Color(0, 137, 137, 137));
		}

		label->Render(renderer);
	}
	//---------------------------------------------------------------------------
	void CheckBox::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(geometry);
		g.Clear();

		g.FillRectangle(GetBackColor(), RectangleF(PointF(0, 0), SizeF(DefaultCheckBoxSize, DefaultCheckBoxSize)));
		
		g.FillRectangleGradient(ColorRectangle(Color::White(), Color::White() - Color(0, 137, 137, 137)), RectangleF(PointF(1, 1), SizeF(15, 15)));
		g.FillRectangleGradient(ColorRectangle(GetBackColor(), GetBackColor() + Color(0, 55, 55, 55)), RectangleF(PointF(2, 2), SizeF(13, 13)));
		
		if (checked)
		{
			g.FillRectangle(Color::White(), RectangleF(PointF(5, 5), SizeF(7, 7)));
			g.FillRectangleGradient(ColorRectangle(Color::White(), Color::White() - Drawing::Color(0, 137, 137, 137)), RectangleF(PointF(6, 6), SizeF(5, 5)));
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void CheckBox::OnMouseClick(const MouseMessage &mouse)
	{
		Control::OnMouseClick(mouse);

		SetChecked(!GetChecked());
	}
	//---------------------------------------------------------------------------
	bool CheckBox::OnKeyUp(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyUp(keyboard))
		{
			if (keyboard.GetKeyCode() == Key::Space)
			{
				SetChecked(!GetChecked());
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
}