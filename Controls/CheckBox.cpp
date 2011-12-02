#include "CheckBox.hpp"
#include "..\Misc\Exceptions.hpp"

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
		: Control()
	{
		type = CONTROL_CHECKBOX;
		
		autoSize = true;

		label = new Label();
		label->SetName("CheckBox_Label");
		label->SetLocation(Drawing::Point(DefaultLabelOffset.Width, DefaultLabelOffset.Height));
		SetText("CheckBox");

		checked = false;

		SetBackColor(Drawing::Color(0xFF222222));
		SetForeColor(Drawing::Color(0xFFE5E0E4));

		canRaiseEvents = true;
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
				checkBoxPosition = Drawing::Point(0, 0);
				int y = (int)(DefaultCheckBoxSize / 2.0f - font->GetSize() / 2.0f + 0.5f);
				label->SetLocation(Drawing::Point(DefaultLabelOffset.Width, y));
			}
			else
			{
				label->SetLocation(Drawing::Point(DefaultLabelOffset.Width, DefaultLabelOffset.Height));
				int y = (int)(font->GetSize() / 2.0f - DefaultCheckBoxSize / 2.0f + 0.5f);
				checkBoxPosition = Drawing::Point(0, y);
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
		
		checkBoxAbsolutePosition = absoluteLocation + checkBoxPosition;

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
	void CheckBox::OnKeyUp(const KeyboardMessage &keyboard)
	{
		Control::OnKeyUp(keyboard);

		if (keyboard.KeyCode == Key::Space)
		{
			SetChecked(!GetChecked());
		}
	}
	//---------------------------------------------------------------------------
	void CheckBox::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(checkBoxAbsolutePosition.Left, checkBoxAbsolutePosition.Top, DefaultCheckBoxSize, DefaultCheckBoxSize);

		Drawing::Color white = Drawing::Color::White();
		renderer->SetRenderColor(white);
		renderer->FillGradient(checkBoxAbsolutePosition.Left + 1, checkBoxAbsolutePosition.Top + 1, 15, 15, white - Drawing::Color(0, 137, 137, 137));
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(checkBoxAbsolutePosition.Left + 2, checkBoxAbsolutePosition.Top + 2, 13, 13, backColor + Drawing::Color(0, 55, 55, 55));
		
		renderer->SetRenderColor(white);
		if (checked)
		{
			renderer->Fill(checkBoxAbsolutePosition.Left + 5, checkBoxAbsolutePosition.Top + 5, 7, 7);
			renderer->FillGradient(checkBoxAbsolutePosition.Left + 6, checkBoxAbsolutePosition.Top + 6, 5, 5, white - Drawing::Color(0, 137, 137, 137));
		}
		
		label->Render(renderer);
	}
	//---------------------------------------------------------------------------
}