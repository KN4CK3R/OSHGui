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
		containerPanel->SetLocation(Drawing::Point(2, 2));
		AddSubControl(containerPanel);
		
		ApplyTheme(Application::Instance()->GetTheme());
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
		if (button != nullptr)
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