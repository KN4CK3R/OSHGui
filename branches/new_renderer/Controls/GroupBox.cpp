/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "GroupBox.hpp"
#include "../Misc/Exceptions.hpp"
#include "Label.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	GroupBox::GroupBox()
	{
		type = CONTROL_GROUPBOX;

		captionLabel = new Label();
		captionLabel->SetLocation(Drawing::Point(5, -1));
		captionLabel->SetBackColor(Drawing::Color::Empty());
		AddSubControl(captionLabel);

		containerPanel = new Panel();
		containerPanel->SetLocation(Drawing::Point(3, 10));
		containerPanel->SetBackColor(Drawing::Color::Empty());
		AddSubControl(containerPanel);

		ApplyTheme(Application::Instance()->GetTheme());

		canRaiseEvents = false;
	}
	//---------------------------------------------------------------------------
	GroupBox::~GroupBox()
	{

	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void GroupBox::SetSize(const Drawing::Size &size)
	{
		ContainerControl::SetSize(size);

		containerPanel->SetSize(size.InflateEx(-3 * 2, -3 * 2 - 10));
	}
	//---------------------------------------------------------------------------
	void GroupBox::SetText(const Misc::AnsiString &text)
	{
		captionLabel->SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& GroupBox::GetText() const
	{
		return captionLabel->GetText();
	}
	//---------------------------------------------------------------------------
	void GroupBox::SetFont(const std::shared_ptr<Drawing::IFont> &font)
	{
		Control::SetFont(font);

		captionLabel->SetFont(font);
	}
	//---------------------------------------------------------------------------
	void GroupBox::SetForeColor(Drawing::Color color)
	{
		Control::SetForeColor(color);

		captionLabel->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	const std::deque<Control*>& GroupBox::GetControls() const
	{
		return containerPanel->GetControls();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool GroupBox::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void GroupBox::AddControl(Control *control)
	{
		containerPanel->AddControl(control);
	}
	//---------------------------------------------------------------------------
	void GroupBox::RemoveControl(Control *control)
	{
		containerPanel->RemoveControl(control);
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
			renderer->Fill(absoluteLocation, size);
		}
		
		captionLabel->Render(renderer);

		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top + 5, 3, 1);
		renderer->Fill(absoluteLocation.Left + captionLabel->GetWidth() + 5, absoluteLocation.Top + 5, GetWidth() - captionLabel->GetWidth() - 6, 1);
		renderer->Fill(absoluteLocation.Left, absoluteLocation.Top + 6, 1, GetHeight() - 7);
		renderer->Fill(absoluteLocation.Left + GetWidth() - 1, absoluteLocation.Top + 6, 1, GetHeight() - 7);
		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top + GetHeight() - 1, GetWidth() - 2, 1);

		containerPanel->Render(renderer);
	}
	//---------------------------------------------------------------------------
}