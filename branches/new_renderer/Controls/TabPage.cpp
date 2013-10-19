/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "TabPage.hpp"
#include "TabControl.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TabPage::TabPage()
		: button(nullptr)
	{
		type = CONTROL_TABPAGE;
	
		containerPanel = new Panel();
		containerPanel->SetLocation(Drawing::PointF(2, 2));
		containerPanel->SetBackColor(Drawing::Color::Empty());
		AddSubControl(containerPanel);
		
		ApplyTheme(Application::Instance()->GetTheme());
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TabPage::SetSize(const Drawing::SizeF &size)
	{
		ContainerControl::SetSize(size);

		containerPanel->SetSize(size.InflateEx(-4, -4));
	}
	//---------------------------------------------------------------------------
	void TabPage::SetParent(Control *parent)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (parent == nullptr)
		{
			throw Misc::ArgumentNullException("parent", __FILE__, __LINE__);
		}
		if (parent->GetType() != CONTROL_TABCONTROL)
		{
			throw Misc::ArgumentException("parent", __FILE__, __LINE__);
		}
		#endif
	
		Panel::SetParent(parent);
	}
	//---------------------------------------------------------------------------
	void TabPage::SetText(const Misc::AnsiString &text)
	{
		if (button)
		{
			button->SetText(text);
		}
		this->text = text;

		Invalidate();
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& TabPage::GetText() const
	{
		return text;
	}
	//---------------------------------------------------------------------------
	const std::deque<Control*>& TabPage::GetControls() const
	{
		return containerPanel->GetControls();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void TabPage::AddControl(Control *control)
	{
		containerPanel->AddControl(control);
	}
	//---------------------------------------------------------------------------
	void TabPage::RemoveControl(Control *control)
	{
		containerPanel->RemoveControl(control);
	}
	//---------------------------------------------------------------------------
	void TabPage::DrawSelf(Drawing::RenderContext &context)
	{
		ContainerControl::DrawSelf(context);

		containerPanel->Render();
	}
	//---------------------------------------------------------------------------
	void TabPage::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(geometry);

		if (backColor.A > 0)
		{
			g.FillRectangle(GetBackColor() + Color(0, 32, 32, 32), PointF(0, 0), GetSize());
			g.FillRectangleGradient(ColorRectangle(GetBackColor(), GetBackColor() - Color(0, 20, 20, 20)), PointF(1, 1), GetSize() - SizeF(2, 2));
		}
	}
	//---------------------------------------------------------------------------
}
