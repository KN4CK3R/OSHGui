/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
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
	{
		type = CONTROL_TABPAGE;

		button = 0;

		containerPanel = new Panel();
		containerPanel->SetLocation(Drawing::Point(2, 2));
		AddSubControl(containerPanel);
		
		SetBackColor(Drawing::Color(0xFF474747));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TabPage::SetSize(const Drawing::Size &size)
	{
		ContainerControl::SetSize(size);

		containerPanel->SetSize(size.InflateEx(-4, -4));
	}
	//---------------------------------------------------------------------------
	void TabPage::SetParent(Control *parent)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (parent == 0)
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
		if (button != 0)
		{
			button->SetText(text);
		}
		this->text = text;
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& TabPage::GetText() const
	{
		return text;
	}
	//---------------------------------------------------------------------------
	const std::vector<Control*>& TabPage::GetControls() const
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
	//Event-Handling
	//---------------------------------------------------------------------------
	void TabPage::Render(Drawing::IRenderer *renderer)
	{
		if (backColor.A > 0)
		{
			renderer->SetRenderColor(backColor + Drawing::Color(0, 32, 32, 32));
			renderer->Fill(absoluteLocation, size);
			renderer->SetRenderColor(backColor);
			renderer->FillGradient(absoluteLocation.Left + 1, absoluteLocation.Top + 1, GetWidth() - 2, GetHeight() - 2, backColor - Drawing::Color(0, 20, 20, 20));
		}
	
		containerPanel->Render(renderer);
	}
	//---------------------------------------------------------------------------
}