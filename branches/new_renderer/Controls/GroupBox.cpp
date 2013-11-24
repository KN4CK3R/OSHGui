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
		type = ControlType::GroupBox;

		captionLabel = new Label();
		captionLabel->SetLocation(Drawing::PointF(5, -1));
		captionLabel->SetBackColor(Drawing::Color::Empty());
		AddSubControl(captionLabel);

		containerPanel = new Panel();
		containerPanel->SetLocation(Drawing::PointF(3, 10));
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
	void GroupBox::SetSize(const Drawing::SizeF &size)
	{
		Control::SetSize(size);

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
	void GroupBox::SetFont(const Drawing::FontPtr &font)
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
	bool GroupBox::Intersect(const Drawing::PointF &point) const
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
	void GroupBox::DrawSelf(Drawing::RenderContext &context)
	{
		Control::DrawSelf(context);

		captionLabel->Render();
		containerPanel->Render();
	}
	//---------------------------------------------------------------------------
	void GroupBox::PopulateGeometry()
	{
		using namespace Drawing;
		
		Graphics g(*geometry);

		if (backColor.A != 0)
		{
			g.FillRectangle(GetBackColor(), PointF(0, 0), GetSize());
		}

		g.FillRectangle(GetForeColor(), PointF(1, 5), SizeF(3, 1));
		g.FillRectangle(GetForeColor(), PointF(5 + captionLabel->GetWidth(), 5), SizeF(GetWidth() - captionLabel->GetWidth() - 6, 1));
		g.FillRectangle(GetForeColor(), PointF(0, 6), SizeF(1, GetHeight() - 7));
		g.FillRectangle(GetForeColor(), PointF(GetWidth() - 1, 6), SizeF(1, GetHeight() - 7));
		g.FillRectangle(GetForeColor(), PointF(1, GetHeight() - 1), SizeF(GetWidth() - 2, 1));
	}
	//---------------------------------------------------------------------------
}
