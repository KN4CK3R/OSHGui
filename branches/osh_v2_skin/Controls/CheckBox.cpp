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
		: checked(false),
		  label(new Label())
	{
		type_ = ControlType::CheckBox;
		
		SetSize(DefaultCheckBoxSize, DefaultCheckBoxSize);
		SetAutoSize(true);

		label->SetLocation(DefaultLabelOffset);
		label->SetBackColor(Drawing::Color::Empty());

		ApplyStyle(Application::Instance().GetStyle());
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void CheckBox::SetChecked(bool _checked)
	{
		if (checked != _checked)
		{
			checked = _checked;
			
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
		auto offset = label->GetLocation();

		label->SetText(text);
		if (autoSize_)
		{
			size_ = label->GetSize().InflateEx(offset.Left, offset.Top);
			if (size_.Height < DefaultCheckBoxSize)
			{
				size_.Height = DefaultCheckBoxSize;
			}
		}
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& CheckBox::GetText() const
	{
		return label->GetText();
	}
	//---------------------------------------------------------------------------
	void CheckBox::SetFont(const Drawing::FontPtr &font)
	{
		Control::SetFont(font);

		label->SetFont(font);
		if (autoSize_)
		{
			size_ = label->GetSize();
			if (GetFont()->GetFontHeight() < DefaultCheckBoxSize)
			{
				checkBoxLocation = Drawing::PointI(0, 1);
				int y = (int)(DefaultCheckBoxSize / 2.0f - GetFont()->GetFontHeight() / 2.0f + 0.5f);
				label->SetLocation(Drawing::PointI(DefaultLabelOffset.Left, y));
			}
			else
			{
				label->SetLocation(DefaultLabelOffset);
				int y = (int)(GetFont()->GetFontHeight() / 2.0f - DefaultCheckBoxSize / 2.0f + 0.5f);
				checkBoxLocation = Drawing::PointI(0, y);
			}
		}
	}
	//---------------------------------------------------------------------------
	void CheckBox::SetForeColor(const Drawing::Color &color)
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
	void CheckBox::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();

		checkBoxAbsoluteLocation = absoluteLocation_ + checkBoxLocation;

		label->SetParent(this);
	}
	//---------------------------------------------------------------------------
	void CheckBox::DrawSelf(Drawing::RenderContext &context)
	{
		Control::DrawSelf(context);

		label->Render();
	}
	//---------------------------------------------------------------------------
	void CheckBox::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry_);

		g.FillRectangle(Drawing::Color(0xFFE1E1E1), RectangleF(PointF(0, 0), SizeF(DefaultCheckBoxSize, DefaultCheckBoxSize)));
		g.FillRectangle(GetBackColor(), RectangleF(PointF(1, 1), SizeF(DefaultCheckBoxSize - 2, DefaultCheckBoxSize - 2)));

		if (checked)
		{
			g.FillRectangle(GetForeColor(), RectangleF(PointF(4, 4), SizeF(7, 7)));
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