/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "CheckBox.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::Size CheckBox::DefaultLabelOffset(20, 2);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	CheckBox::CheckBox()
		: checked(false)
	{
		type = CONTROL_CHECKBOX;
		
		SetAutoSize(true);

		label = new Label();
		label->SetLocation(Drawing::Point(DefaultLabelOffset.Width, DefaultLabelOffset.Height));

		ApplyTheme(Application::Instance()->GetTheme());
	}
	//---------------------------------------------------------------------------
	CheckBox::~CheckBox()
	{
		delete label;
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
		Drawing::Point offset = label->GetLocation();

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
				checkBoxLocation = Drawing::Point(0, 0);
				int y = (int)(DefaultCheckBoxSize / 2.0f - font->GetSize() / 2.0f + 0.5f);
				label->SetLocation(Drawing::Point(DefaultLabelOffset.Width, y));
			}
			else
			{
				label->SetLocation(Drawing::Point(DefaultLabelOffset.Width, DefaultLabelOffset.Height));
				int y = (int)(font->GetSize() / 2.0f - DefaultCheckBoxSize / 2.0f + 0.5f);
				checkBoxLocation = Drawing::Point(0, y);
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
	bool CheckBox::Intersect(const Drawing::Point &point) const
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
			if (keyboard.KeyCode == Key::Space)
			{
				SetChecked(!GetChecked());
			}
		}

		return true;
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
}