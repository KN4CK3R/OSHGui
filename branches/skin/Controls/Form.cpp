/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "Form.hpp"
#include "../Application.hpp"
#include "../FormManager.hpp"
#include "../Misc/Exceptions.hpp"
#include "Label.hpp"
#include "Panel.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::PointI Form::DefaultLocation(50, 50);
	const Drawing::SizeI Form::DefaultSize(300, 300);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Form::Form()
		: isModal_(false),
		  dialogResult_(DialogResult::None)
	{
		type_ = ControlType::Form;
		
		parent_ = nullptr;//this;
		isVisible_ = false;
		isEnabled_ = false;
		isFocusable_ = true;

		captionBar_ = new CaptionBar();
		captionBar_->SetLocation(Drawing::PointI(0, 0));
		AddSubControl(captionBar_);

		containerPanel_ = new Panel();
		containerPanel_->SetLocation(Drawing::PointI(DefaultBorderPadding, DefaultBorderPadding + CaptionBar::DefaultCaptionBarHeight));
		containerPanel_->SetBackColor(Drawing::Color::Empty());
		AddSubControl(containerPanel_);

		SetLocation(DefaultLocation);
		SetSize(DefaultSize);

		ApplyStyle(Application::Instance().GetStyle());
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Form::SetSize(const Drawing::SizeI &size)
	{
		Control::SetSize(size);

		captionBar_->SetSize(size);
		containerPanel_->SetSize(size.InflateEx(-DefaultBorderPadding * 2, -DefaultBorderPadding * 2 - CaptionBar::DefaultCaptionBarHeight));
	}
	//---------------------------------------------------------------------------
	void Form::SetText(const Misc::AnsiString &text)
	{
		captionBar_->SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& Form::GetText() const
	{
		return captionBar_->GetText();
	}
	//---------------------------------------------------------------------------
	void Form::SetForeColor(const Drawing::Color &color)
	{
		Control::SetForeColor(color);

		captionBar_->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	const std::deque<Control*>& Form::GetControls() const
	{
		return containerPanel_->GetControls();
	}
	//---------------------------------------------------------------------------
	void Form::SetDialogResult(DialogResult result)
	{
		dialogResult_ = result;
	}
	//---------------------------------------------------------------------------
	DialogResult Form::GetDialogResult() const
	{
		return dialogResult_;
	}
	//---------------------------------------------------------------------------
	FormClosingEvent& Form::GetFormClosingEvent()
	{
		return formClosingEvent_;
	}
	//---------------------------------------------------------------------------
	bool Form::IsModal() const
	{
		return isModal_;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void Form::AddControl(Control *control)
	{
		containerPanel_->AddControl(control);
	}
	//---------------------------------------------------------------------------
	void Form::Show(const std::shared_ptr<Form> &instance)
	{
		instance_ = std::weak_ptr<Form>(instance);
	
		Application::Instance().formManager_.RegisterForm(instance);

		isVisible_ = true;
		isEnabled_ = true;

		CalculateAbsoluteLocation();
	}
	//---------------------------------------------------------------------------
	void Form::ShowDialog(const std::shared_ptr<Form> &instance)
	{
		ShowDialog(instance, std::function<void()>());
	}
	//---------------------------------------------------------------------------
	void Form::ShowDialog(const std::shared_ptr<Form> &instance, const std::function<void()> &closeFunction)
	{
		isModal_ = true;

		instance_ = std::weak_ptr<Form>(instance);
	
		Application::Instance().formManager_.RegisterForm(instance_.lock(), closeFunction);

		isVisible_ = true;
		isEnabled_ = true;

		CalculateAbsoluteLocation();
	}
	//---------------------------------------------------------------------------
	void Form::Close()
	{
		bool canClose = true;
		formClosingEvent_.Invoke(this, canClose);
		if (canClose)
		{
			Application::Instance().formManager_.UnregisterForm(instance_.lock());
		}
	}
	//---------------------------------------------------------------------------
	void Form::DrawSelf(Drawing::RenderContext &context, Skins::Base &skin)
	{
		Control::DrawSelf(context, skin);

		captionBar_->Render(skin);
		containerPanel_->Render(skin);
	}
	//---------------------------------------------------------------------------
	void Form::PopulateGeometry(Skins::Base &skin)
	{
		skin.DrawForm(Drawing::Graphics(*geometry_), this, captionBar_->GetBottom());
	}
	//---------------------------------------------------------------------------
	//Form::Captionbar::Button
	//---------------------------------------------------------------------------
	const Drawing::SizeI Form::CaptionBar::CaptionBarButton::DefaultSize(17, 17);
	const Drawing::PointI Form::CaptionBar::CaptionBarButton::DefaultCrossOffset(8, 6);
	//---------------------------------------------------------------------------
	Form::CaptionBar::CaptionBarButton::CaptionBarButton()
	{
		isFocusable_ = false;

		SetSize(DefaultSize);
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::CaptionBarButton::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();

		crossAbsoluteLocation_ = absoluteLocation_ + DefaultCrossOffset;

		geometry_->SetTranslation(Drawing::Vector(crossAbsoluteLocation_.X, crossAbsoluteLocation_.Y, 0.0f));
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::CaptionBarButton::OnMouseUp(const MouseMessage &mouse)
	{
		Control::OnMouseUp(mouse);

		auto owner = static_cast<Form*>(parent_->GetParent());
		owner->Close();
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::CaptionBarButton::PopulateGeometry(Skins::Base &skin)
	{
		skin.DrawCaptionBarButton(Drawing::Graphics(*geometry_), this);
	}
	//---------------------------------------------------------------------------
	//Form::Captionbar
	//---------------------------------------------------------------------------
	const Drawing::PointI Form::CaptionBar::DefaultTitleOffset(4, 4);
	//---------------------------------------------------------------------------
	Form::CaptionBar::CaptionBar()
	{
		isFocusable_ = false;
		drag_ = false;

		titleLabel_ = new Label();
		titleLabel_->SetLocation(DefaultTitleOffset);
		titleLabel_->SetBackColor(Drawing::Color::Empty());

		AddSubControl(titleLabel_);

		closeButton_ = new CaptionBarButton();
		closeButton_->SetBackColor(Drawing::Color::Empty());
		
		AddSubControl(closeButton_);
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::SetSize(const Drawing::SizeI &size)
	{
		Control::SetSize(Drawing::SizeI(size.Width, DefaultCaptionBarHeight));

		closeButton_->SetLocation(Drawing::PointI(size.Width - CaptionBarButton::DefaultSize.Width - DefaultButtonPadding, 0));
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::SetText(const Misc::AnsiString &text)
	{
		titleLabel_->SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& Form::CaptionBar::GetText() const
	{
		return titleLabel_->GetText();
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::SetForeColor(const Drawing::Color &color)
	{
		Control::SetForeColor(color);

		closeButton_->SetForeColor(color);
		titleLabel_->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::DrawSelf(Drawing::RenderContext &context, Skins::Base &skin)
	{
		Control::DrawSelf(context, skin);

		titleLabel_->Render(skin);
		closeButton_->Render(skin);
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::OnMouseDown(const MouseMessage &mouse)
	{
		Control::OnMouseDown(mouse);

		drag_ = true;
		OnGotMouseCapture();
		dragStart_ = mouse.GetLocation();
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::OnMouseMove(const MouseMessage &mouse)
	{
		Control::OnMouseMove(mouse);
		
		if (drag_)
		{
			GetParent()->SetLocation(GetParent()->GetLocation() + (mouse.GetLocation() - dragStart_));
			dragStart_ = mouse.GetLocation();
		}
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::OnMouseUp(const MouseMessage &mouse)
	{
		Control::OnMouseUp(mouse);

		if (drag_)
		{
			drag_ = false;
			OnLostMouseCapture();
		}
	}
	//---------------------------------------------------------------------------
}
