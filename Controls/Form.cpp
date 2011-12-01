#include "Form.hpp"
#include "..\Application.hpp"
#include "..\FormManager.hpp"
#include "..\Misc\Exceptions.hpp"
#include "Label.hpp"

namespace OSHGui
{
	const Drawing::Point Form::DefaultLocation(50, 50);
	const Drawing::Size Form::DefaultSize(300, 300);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Form::Form(const Misc::AnsiString &name, const Drawing::Point &location, const Drawing::Size &size, const Misc::AnsiString &text)
		: Panel(name, location, size), textHelper(font)
	{
		parent = this;

		type = CONTROL_FORM;
		
		isVisible = false;
		isEnabled = false;
		isModal = false;
		isFocusable = true;

		dialogResult = ResultNone;

		SetBackColor(Drawing::Color(0xFF7c7b79));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
		
		captionBar = new CaptionBar(name + "_CaptionBar", Drawing::Point(), Drawing::Size(size.Width, CaptionBar::DefaultCaptionBarHeight), text);
		AddSubControl(captionBar);
		containerPanel = new Panel(name + "_ContainerPanel", Drawing::Point(6, 6 + captionBar->GetHeight()), size.InflateEx(-12, -6 - captionBar->GetHeight() - 6));
		AddSubControl(containerPanel);
	}
	//---------------------------------------------------------------------------
	Form::~Form()
	{

	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Form::SetText(const Misc::AnsiString &text)
	{
		textHelper.SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& Form::GetText() const
	{
		return textHelper.GetText();
	}
	//---------------------------------------------------------------------------
	DialogResult Form::GetDialogResult() const
	{
		return dialogResult;
	}
	//---------------------------------------------------------------------------
	bool Form::IsModal() const
	{
		return isModal;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void Form::Invalidate()
	{
		//captionBar = Drawing::Rectangle(bounds.GetLeft() + 1, bounds.GetTop() + 1, bounds.GetWidth() - 23, 17);
		//closeRect = Drawing::Rectangle(captionBar.GetRight(), bounds.GetTop() + 2, 17, 17);

		clientArea = Drawing::Rectangle(bounds.GetLeft() + 3, bounds.GetTop() + 20, bounds.GetWidth() - 6, bounds.GetHeight() - 23);

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	void Form::AddControl(Control *control)
	{
		containerPanel->AddControl(control);
	}
	//---------------------------------------------------------------------------
	void Form::Show(const std::shared_ptr<Form> &instance)
	{
		this->instance = std::weak_ptr<Form>(instance);
	
		Application::Instance()->formManager.RegisterForm(this->instance.lock());

		isVisible = true;
		isEnabled = true;

		CalculateAbsoluteLocation();
	}
	//---------------------------------------------------------------------------
	void Form::ShowDialog(const std::shared_ptr<Form> &instance)
	{
		ShowDialog(instance, 0);
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
		Application::Instance()->formManager.UnregisterForm(instance.lock());
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------
	void Form::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		renderer->SetRenderColor(backColor - Drawing::Color(0, 100, 100, 100));
		renderer->Fill(location, size);
		renderer->SetRenderColor(backColor);
		//renderer->FillGradient(bounds.GetLeft() + 1, bounds.GetTop() + 1, bounds.GetWidth() - 2, bounds.GetHeight() - 2, backColor - Drawing::Color(90, 90, 90));
		renderer->SetRenderColor(backColor - Drawing::Color(0, 50, 50, 50));
		//renderer->Fill(bounds.GetLeft() + 5, captionBar.GetBottom() + 2, bounds.GetWidth() - 10, 1);

		
	}
	//---------------------------------------------------------------------------
	//Form::Captionbar::Button
	//---------------------------------------------------------------------------
	const Drawing::Point Form::CaptionBar::CaptionBarButton::DefaultCrossOffset(4, 4);
	//---------------------------------------------------------------------------
	Form::CaptionBar::CaptionBarButton::CaptionBarButton(const Misc::AnsiString &name, const Drawing::Point &location)
		: Control(name, location, Drawing::Size(DefaultButtonWidth, DefaultButtonHeight))
	{
		isFocusable = false;
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::CaptionBarButton::OnMouseUp(const MouseMessage &mouse)
	{
		Control::OnMouseUp(mouse);

		Form *owner = (Form*)parent->GetParent();
		owner->Close();
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::CaptionBarButton::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();
		crossAbsoluteLocation = absoluteLocation + DefaultCrossOffset;
	}
	//---------------------------------------------------------------------------
	bool Form::CaptionBar::CaptionBarButton::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::CaptionBarButton::Render(Drawing::IRenderer *renderer)
	{
		for (int i = 0; i < 4; ++i)
		{
			renderer->Fill(crossAbsoluteLocation.Left + 4 + i, crossAbsoluteLocation.Top + 4 + i, 3, 1);
			renderer->Fill(crossAbsoluteLocation.Left + 10 - i, crossAbsoluteLocation.Top + 4 + i, 3, 1);
			renderer->Fill(crossAbsoluteLocation.Left + 4 + i, crossAbsoluteLocation.Top + 11 - i, 3, 1);
			renderer->Fill(crossAbsoluteLocation.Left + 10 - i, crossAbsoluteLocation.Top + 11 - i, 3, 1);
		}
	}
	//---------------------------------------------------------------------------
	//Form::Captionbar
	//---------------------------------------------------------------------------
	const Drawing::Point Form::CaptionBar::DefaultTitleOffset(4, 2);
	//---------------------------------------------------------------------------
	Form::CaptionBar::CaptionBar(const Misc::AnsiString &name, const Drawing::Point &location, const Drawing::Size &size, const Misc::AnsiString &text)
		: ContainerControl(name, location, size)
	{
		isFocusable = false;
		drag = false;

		titleLabel = new Label(name + "_Label", DefaultTitleOffset, Drawing::Size(), text);
		closeButton = new CaptionBarButton(name + "_CaptionBarButton", Drawing::Point(size.Width - CaptionBarButton::DefaultButtonWidth - DefaultButtonPadding, 0));

		AddSubControl(titleLabel);
		AddSubControl(closeButton);
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
	void Form::CaptionBar::OnMouseDown(const MouseMessage &mouse)
	{
		ContainerControl::OnMouseDown(mouse);

		drag = true;
		Application::Instance()->CaptureControl = this;
		dragStart = mouse.Position;
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::OnMouseMove(const MouseMessage &mouse)
	{
		ContainerControl::OnMouseMove(mouse);
		
		if (drag)
		{
			GetParent()->SetLocation(GetParent()->GetLocation() + (mouse.Position - dragStart));
			dragStart = mouse.Position;
		}
	}
	//---------------------------------------------------------------------------
	void Form::CaptionBar::OnMouseClick(const MouseMessage &mouse)
	{
		ContainerControl::OnMouseClick(mouse);
		if (drag)
		{
			drag = false;
			OnMouseCaptureChanged();
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