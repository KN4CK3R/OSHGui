/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
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
	const Drawing::PointF Form::DefaultLocation(50, 50);
	const Drawing::SizeF Form::DefaultSize(300, 300);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Form::Form()
		: isModal(false),
		  dialogResult(DialogResult::ResultNone)
	{
		type = CONTROL_FORM;
		
		parent = nullptr;//this;
		isVisible = false;
		isEnabled = false;
		isFocusable = true;

		captionBar = new CaptionBar();
		captionBar->SetLocation(Drawing::PointF(0, 0));
		AddSubControl(captionBar);

		containerPanel = new Panel();
		containerPanel->SetLocation(Drawing::PointF(DefaultBorderPadding, DefaultBorderPadding + CaptionBar::DefaultCaptionBarHeight));
		containerPanel->SetBackColor(Drawing::Color::Empty());
		AddSubControl(containerPanel);

		SetLocation(DefaultLocation);
		SetSize(DefaultSize);

		ApplyTheme(Application::Instance()->GetTheme());
	}
	//---------------------------------------------------------------------------
	Form::~Form()
	{

	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Form::SetSize(const Drawing::SizeF &size)
	{
		ContainerControl::SetSize(size);

		captionBar->SetSize(size);
		containerPanel->SetSize(size.InflateEx(-DefaultBorderPadding * 2, -DefaultBorderPadding * 2 - CaptionBar::DefaultCaptionBarHeight));
	}
	//---------------------------------------------------------------------------
	void Form::SetText(const Misc::AnsiString &text)
	{
		captionBar->SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& Form::GetText() const
	{
		return captionBar->GetText();
	}
	//---------------------------------------------------------------------------
	void Form::SetForeColor(Drawing::Color color)
	{
		ContainerControl::SetForeColor(color);

		captionBar->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	const std::deque<Control*>& Form::GetControls() const
	{
		return containerPanel->GetControls();
	}
	//---------------------------------------------------------------------------
	DialogResult Form::GetDialogResult() const
	{
		return dialogResult;
	}
	//---------------------------------------------------------------------------
	FormClosingEvent& Form::GetFormClosingEvent()
	{
		return formClosingEvent;
	}
	//---------------------------------------------------------------------------
	bool Form::IsModal() const
	{
		return isModal;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void Form::AddControl(Control *control)
	{
		containerPanel->AddControl(control);
	}
	//---------------------------------------------------------------------------
	void Form::Show(const std::shared_ptr<Form> &instance)
	{
		this->instance = std::weak_ptr<Form>(instance);
	
		Application::Instance()->formManager.RegisterForm(instance);

		isVisible = true;
		isEnabled = true;

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
		isModal = true;

		this->instance = std::weak_ptr<Form>(instance);
	
		Application::Instance()->formManager.RegisterForm(this->instance.lock(), closeFunction);

		isVisible = true;
		isEnabled = true;

		CalculateAbsoluteLocation();
	}
	//---------------------------------------------------------------------------
	void Form::Close()
	{
		bool canClose = true;
		formClosingEvent.Invoke(this, canClose);
		if (canClose)
		{
			Application::Instance()->formManager.UnregisterForm(instance.lock());
		}
	}
	//---------------------------------------------------------------------------
	void Form::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		renderer->SetRenderColor(backColor - Drawing::Color(0, 100, 100, 100));
		renderer->Fill(absoluteLocation, size);
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(absoluteLocation.Left + 1, absoluteLocation.Top + 1, size.Width - 2, size.Height - 2, backColor - Drawing::Color(90, 90, 90));
		renderer->SetRenderColor(backColor - Drawing::Color(0, 50, 50, 50));
		renderer->Fill(absoluteLocation.Left + 5, absoluteLocation.Top + captionBar->GetBottom() + 2, size.Width - 10, 1);

		captionBar->Render(renderer);
		containerPanel->Render(renderer);
	}
	//---------------------------------------------------------------------------
	void Form::DrawSelf(Drawing::RenderContext &context)
	{
		ContainerControl::DrawSelf(context);

		captionBar->Render_();
		containerPanel->Render_();
	}
	//---------------------------------------------------------------------------
	void Form::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(geometry);
		g.Clear();

		g.FillRectangle(backColor - Drawing::Color(0, 100, 100, 100), RectangleF(PointF(), GetSize()));
		auto color = GetBackColor() - Color(0, 90, 90, 90);
		g.FillRectangleGradient(ColorRectangle(GetBackColor(), GetBackColor(), color, color), RectangleF(PointF(1, 1), GetSize() - SizeF(2, 2)));
		g.FillRectangle(backColor - Drawing::Color(0, 50, 50, 50), RectangleF(PointF(5, captionBar->GetBottom() + 2), SizeF(GetWidth() - 10, 1)));
	}
	//---------------------------------------------------------------------------
	//Form::Captionbar::Button
	//---------------------------------------------------------------------------
	const Drawing::PointF Form::CaptionBar::CaptionBarButton::DefaultCrossOffset(8, 6);
	//---------------------------------------------------------------------------
	Form::CaptionBar::CaptionBarButton::CaptionBarButton()
		: Control()
	{
		isFocusable = false;

		SetSize(Drawing::SizeF(DefaultButtonWidth, DefaultButtonHeight));
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::CaptionBarButton::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();
		crossAbsoluteLocation = absoluteLocation + DefaultCrossOffset;

		geometry->SetTranslation(Drawing::Vector(crossAbsoluteLocation.X, crossAbsoluteLocation.Y, 0.0f));
	}
	//---------------------------------------------------------------------------
	bool Form::CaptionBar::CaptionBarButton::Intersect(const Drawing::PointF &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::CaptionBarButton::OnMouseUp(const MouseMessage &mouse)
	{
		Control::OnMouseUp(mouse);

		Form *owner = static_cast<Form*>(parent->GetParent());
		owner->Close();
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::CaptionBarButton::Render(Drawing::IRenderer *renderer)
	{
		for (int i = 0; i < 4; ++i)
		{
			renderer->Fill(crossAbsoluteLocation.Left + i, crossAbsoluteLocation.Top + i, 3, 1);
			renderer->Fill(crossAbsoluteLocation.Left + 6 - i, crossAbsoluteLocation.Top + i, 3, 1);
			renderer->Fill(crossAbsoluteLocation.Left + i, crossAbsoluteLocation.Top + 7 - i, 3, 1);
			renderer->Fill(crossAbsoluteLocation.Left + 6 - i, crossAbsoluteLocation.Top + 7 - i, 3, 1);
		}
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::CaptionBarButton::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(geometry);
		g.Clear();

		auto color = GetParent()->GetForeColor();

		for (int i = 0; i < 4; ++i)
		{
			g.FillRectangle(color, PointF(i, i), SizeF(3, 1));
			g.FillRectangle(color, PointF(6 - i, i), SizeF(3, 1));
			g.FillRectangle(color, PointF(i, 7 - i), SizeF(3, 1));
			g.FillRectangle(color, PointF(6 - i, 7 - i), SizeF(3, 1));
		}
	}
	//---------------------------------------------------------------------------
	//Form::Captionbar
	//---------------------------------------------------------------------------
	const Drawing::PointF Form::CaptionBar::DefaultTitleOffset(4, 4);
	//---------------------------------------------------------------------------
	Form::CaptionBar::CaptionBar()
		: ContainerControl()
	{
		isFocusable = false;
		drag = false;

		titleLabel = new Label();
		titleLabel->SetLocation(DefaultTitleOffset);
		titleLabel->SetBackColor(Drawing::Color::Empty());

		AddSubControl(titleLabel);

		closeButton = new CaptionBarButton();
		closeButton->SetBackColor(Drawing::Color::Empty());
		
		AddSubControl(closeButton);
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::SetSize(const Drawing::SizeF &size)
	{
		ContainerControl::SetSize(Drawing::SizeF(size.Width, DefaultCaptionBarHeight));

		closeButton->SetLocation(Drawing::PointF(size.Width - CaptionBarButton::DefaultButtonWidth - DefaultButtonPadding, 0));
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::SetText(const Misc::AnsiString &text)
	{
		titleLabel->SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& Form::CaptionBar::GetText() const
	{
		return titleLabel->GetText();
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::SetForeColor(Drawing::Color color)
	{
		ContainerControl::SetForeColor(color);

		closeButton->SetForeColor(color);
		titleLabel->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::DrawSelf(Drawing::RenderContext &context)
	{
		BufferGeometry(context);
		QueueGeometry(context);

		titleLabel->Render_();
		closeButton->Render_();
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::OnMouseDown(const MouseMessage &mouse)
	{
		ContainerControl::OnMouseDown(mouse);

		drag = true;
		OnGotMouseCapture();
		dragStart = mouse.Location;
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::OnMouseMove(const MouseMessage &mouse)
	{
		ContainerControl::OnMouseMove(mouse);
		
		if (drag)
		{
			GetParent()->SetLocation(GetParent()->GetLocation() + (mouse.Location - dragStart));
			dragStart = mouse.Location;
		}
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::OnMouseUp(const MouseMessage &mouse)
	{
		ContainerControl::OnMouseUp(mouse);
		if (drag)
		{
			drag = false;
			OnLostMouseCapture();
		}
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::Render(Drawing::IRenderer *renderer)
	{
		titleLabel->Render(renderer);
		closeButton->Render(renderer);
	}
	//---------------------------------------------------------------------------
}