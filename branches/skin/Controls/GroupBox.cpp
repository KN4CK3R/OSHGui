/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
		type_ = ControlType::GroupBox;

		captionLabel_ = new Label();
		captionLabel_->SetLocation(Drawing::PointI(5, -1));
		captionLabel_->SetBackColor(Drawing::Color::Empty());
		AddSubControl(captionLabel_);

		containerPanel_ = new Panel();
		containerPanel_->SetLocation(Drawing::PointI(3, 10));
		containerPanel_->SetBackColor(Drawing::Color::Empty());
		AddSubControl(containerPanel_);

		ApplyStyle(Application::Instance().GetStyle());

		canRaiseEvents_ = false;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void GroupBox::SetSize(const Drawing::SizeI &size)
	{
		Control::SetSize(size);

		containerPanel_->SetSize(size.InflateEx(-3 * 2, -3 * 2 - 10));
	}
	//---------------------------------------------------------------------------
	void GroupBox::SetText(const Misc::AnsiString &text)
	{
		captionLabel_->SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& GroupBox::GetText() const
	{
		return captionLabel_->GetText();
	}
	//---------------------------------------------------------------------------
	void GroupBox::SetFont(const Drawing::FontPtr &font)
	{
		Control::SetFont(font);

		captionLabel_->SetFont(font);
	}
	//---------------------------------------------------------------------------
	void GroupBox::SetForeColor(const Drawing::Color &color)
	{
		Control::SetForeColor(color);

		captionLabel_->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	const std::deque<Control*>& GroupBox::GetControls() const
	{
		return containerPanel_->GetControls();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void GroupBox::AddControl(Control *control)
	{
		containerPanel_->AddControl(control);
	}
	//---------------------------------------------------------------------------
	void GroupBox::RemoveControl(Control *control)
	{
		containerPanel_->RemoveControl(control);
	}
	//---------------------------------------------------------------------------
	void GroupBox::DrawSelf(Drawing::RenderContext &context, Skins::Base &skin)
	{
		Control::DrawSelf(context, skin);

		captionLabel_->Render(skin);
		containerPanel_->Render(skin);
	}
	//---------------------------------------------------------------------------
	void GroupBox::PopulateGeometry(Skins::Base &skin)
	{
		skin.DrawGroupBox(Drawing::Graphics(*geometry_), this, captionLabel_->GetWidth());
	}
	//---------------------------------------------------------------------------
}
