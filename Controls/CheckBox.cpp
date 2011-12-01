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
	CheckBox::CheckBox(const Misc::AnsiString &name, const Drawing::Point &location, const Drawing::Size &size, const Misc::AnsiString &text, bool checked) : Control(name, location, size)
	{
		type = CONTROL_CHECKBOX;
		
		autoSize = true;

		label = new Label(name + "_Label", Drawing::Point(DefaultLabelOffset.Width, DefaultLabelOffset.Height), Drawing::Size(), text);

		this->checked = checked;

		SetBackColor(Drawing::Color(0xFF222222));
		SetForeColor(Drawing::Color(0xFFE5E0E4));

		canRaiseEvents = true;
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
		Drawing::Point offset = label->GetLocation() - GetLocation();

		label->SetText(text);
		if (autoSize)
		{
			size = label->GetSize().InflateEx(offset.Left, offset.Top);
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
			if (font->GetSize() < 17)
			{
				checkBoxPosition = location;
				int y = (int)(17 / 2.0f - font->GetSize() / 2.0f + 0.5f);
				textPosition = Drawing::Point(GetLeft() + 20, GetTop() + y);
			}
			else
			{
				textPosition = GetLocation().OffsetEx(20, 0);
				int y = (int)(font->GetSize() / 2.0f - 17 / 2.0f + 0.5f);
				checkBoxPosition = Drawing::Point(GetLeft(), GetTop() + y);
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
	bool CheckBox::CanHaveFocus() const
	{
		return isVisible && isEnabled;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void CheckBox::OnMouseClick(const MouseMessage &mouse)
	{
		SetChecked(!GetChecked());

		Control::OnMouseClick(mouse);
	}
	//---------------------------------------------------------------------------
	void CheckBox::OnKeyUp(const KeyboardMessage &keyboard)
	{
		SetChecked(!GetChecked());

		Control::OnKeyUp(keyboard);
	}
	//---------------------------------------------------------------------------
	void CheckBox::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(checkBoxPosition.Left, checkBoxPosition.Top, 17, 17);

		Drawing::Color white = Drawing::Color::White();
		renderer->SetRenderColor(white);
		renderer->FillGradient(checkBoxPosition.Left + 1, checkBoxPosition.Top + 1, 15, 15, white - Drawing::Color(0, 137, 137, 137));
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(checkBoxPosition.Left + 2, checkBoxPosition.Top + 2, 13, 13, backColor + Drawing::Color(0, 55, 55, 55));
		
		renderer->SetRenderColor(white);
		if (checked)
		{
			renderer->Fill(checkBoxPosition.Left + 5, checkBoxPosition.Top + 5, 7, 7);
			renderer->FillGradient(checkBoxPosition.Left + 6, checkBoxPosition.Top + 6, 5, 5, white - Drawing::Color(0, 137, 137, 137));
		}
		
		renderer->SetRenderColor(foreColor);
//		renderer->RenderText(font, textPosition.Left, textPosition.Top, bounds.GetWidth() - 20, bounds.GetHeight(), textHelper.GetText());

		if (controls.size() > 0)
		{
			Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
			renderer->SetRenderRectangle(clientArea + renderRect.GetPosition());
			
			ChildRender(renderer);
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}