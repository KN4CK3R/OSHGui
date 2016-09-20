/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
	const Drawing::PointI CheckBox::DefaultLabelOffset(20, 2);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	CheckBox::CheckBox()
		: checked_(false),
		  label_(new Label())
	{
		type_ = ControlType::CheckBox;
		
		SetSize(DefaultCheckBoxSize, DefaultCheckBoxSize);
		SetAutoSize(true);

		label_->SetLocation(DefaultLabelOffset);
		label_->SetBackColor(Drawing::Color::Empty());

		ApplyStyle(Application::Instance().GetStyle());
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void CheckBox::SetChecked(bool checked)
	{
		if (checked_ != checked)
		{
			checked_ = checked;
			
			checkedChangedEvent_.Invoke(this);

			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	bool CheckBox::GetChecked() const
	{
		return checked_;
	}
	//---------------------------------------------------------------------------
	void CheckBox::SetText(const Misc::AnsiString &text)
	{
		auto offset = label_->GetLocation();

		label_->SetText(text);
		if (autoSize_)
		{
			size_ = label_->GetSize().InflateEx(offset.Left, offset.Top);
			if (size_.Height < DefaultCheckBoxSize)
			{
				size_.Height = DefaultCheckBoxSize;
			}
		}
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& CheckBox::GetText() const
	{
		return label_->GetText();
	}
	//---------------------------------------------------------------------------
	void CheckBox::SetFont(const Drawing::FontPtr &font)
	{
		Control::SetFont(font);

		label_->SetFont(font);
		if (autoSize_)
		{
			size_ = label_->GetSize();
			if (GetFont()->GetFontHeight() < DefaultCheckBoxSize)
			{
				checkBoxLocation_ = Drawing::PointI(0, 0);
				int y = (int)(DefaultCheckBoxSize / 2.0f - GetFont()->GetFontHeight() / 2.0f + 0.5f);
				label_->SetLocation(Drawing::PointI(DefaultLabelOffset.Left, y));
			}
			else
			{
				label_->SetLocation(DefaultLabelOffset);
				int y = (int)(GetFont()->GetFontHeight() / 2.0f - DefaultCheckBoxSize / 2.0f + 0.5f);
				checkBoxLocation_ = Drawing::PointI(0, y);
			}
		}
	}
	//---------------------------------------------------------------------------
	void CheckBox::SetForeColor(const Drawing::Color &color)
	{
		Control::SetForeColor(color);

		label_->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	CheckedChangedEvent& CheckBox::GetCheckedChangedEvent()
	{
		return checkedChangedEvent_;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void CheckBox::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();

		checkBoxAbsoluteLocation_ = absoluteLocation_ + checkBoxLocation_;

		label_->SetParent(this);
	}
	//---------------------------------------------------------------------------
	void CheckBox::DrawSelf(Drawing::RenderContext &context, Skins::Base &skin)
	{
		Control::DrawSelf(context, skin);

		label_->Render(skin);
	}
	//---------------------------------------------------------------------------
	void CheckBox::PopulateGeometry(Skins::Base &skin)
	{
		skin.DrawCheckBox(Drawing::Graphics(*geometry_), this, Drawing::SizeF(DefaultCheckBoxSize, DefaultCheckBoxSize), checked_);
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