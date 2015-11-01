/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
		type_ = ControlType::TabPage;
	
		containerPanel = new Panel();
		containerPanel->SetLocation(Drawing::PointI(2, 2));
		containerPanel->SetBackColor(Drawing::Color::Empty());
		AddSubControl(containerPanel);
		
		ApplyStyle(Application::Instance().GetStyle());
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TabPage::SetSize(const Drawing::SizeI &_size)
	{
		Control::SetSize(_size);

		containerPanel->SetSize(_size.InflateEx(-4, -4));
	}
	//---------------------------------------------------------------------------
	void TabPage::SetParent(Control *parent)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (parent == nullptr)
		{
			throw Misc::ArgumentNullException("parent");
		}
		if (parent->GetType() != ControlType::TabControl)
		{
			throw Misc::ArgumentException("parent");
		}
		#endif
	
		Panel::SetParent(parent);
	}
	//---------------------------------------------------------------------------
	void TabPage::SetText(const Misc::AnsiString &_text)
	{
		if (button)
		{
			button->SetText(_text);
		}

		text = _text;

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
		Control::DrawSelf(context);

		containerPanel->Render();
	}
	//---------------------------------------------------------------------------
	void TabPage::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry_);

		g.FillRectangle(GetForeColor(), PointF(0, 0), GetSize());
		g.FillRectangle(GetBackColor(), PointF(1, 1), GetSize() - SizeF(2, 2));
	}
	//---------------------------------------------------------------------------
}
