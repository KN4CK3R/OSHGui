#include "GroupBox.hpp"
#include "..\Misc\Exceptions.hpp"
#include "Label.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	GroupBox::GroupBox()
		: Control()
	{
		type = CONTROL_GROUPBOX;

		label = new Label();
		label->SetName("GroupBox_Label");
		label->SetLocation(Drawing::Point(5, -1));
		label->SetText("GroupBox");

		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	GroupBox::~GroupBox()
	{
		delete label;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void GroupBox::SetText(const Misc::AnsiString &text)
	{
		label->SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& GroupBox::GetText() const
	{
		return label->GetText();
	}
	//---------------------------------------------------------------------------
	void GroupBox::SetFont(const std::shared_ptr<Drawing::IFont> &font)
	{
		Control::SetFont(font);

		label->SetFont(font);
	}
	//---------------------------------------------------------------------------
	void GroupBox::SetForeColor(Drawing::Color color)
	{
		Control::SetForeColor(color);

		label->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool GroupBox::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void GroupBox::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(bounds);
		}
		
		renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop() + 5, 3, 1);
		renderer->Fill(bounds.GetLeft() + label->GetWidth() + 5, bounds.GetTop() + 5, bounds.GetWidth() - label->GetWidth() - 6, 1);
		renderer->Fill(bounds.GetLeft(), bounds.GetTop() + 6, 1, bounds.GetHeight() - 7);
		renderer->Fill(bounds.GetLeft() + bounds.GetWidth() - 1, bounds.GetTop() + 6, 1, bounds.GetHeight() - 7);
		renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop() + bounds.GetHeight() - 1, bounds.GetWidth() - 2, 1);
	}
	//---------------------------------------------------------------------------
}