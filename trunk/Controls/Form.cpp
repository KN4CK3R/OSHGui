#include "Form.hpp"
#include "..\Application.hpp"
#include "..\FormManager.hpp"
#include "..\Misc\Exceptions.hpp"
#include <Windows.h>

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Form::Form() : Control(this), textHelper(font)
	{
		type = CONTROL_FORM;
		
		isVisible = false;
		isEnabled = false;
		drag = false;
		isModal = false;

		dialogResult = ResultNone;

		SetBounds(50, 50, 300, 300);

		SetBackColor(Drawing::Color(0xFF7c7b79));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
		SetText(L"Form");

		Invalidate();
	}
	//---------------------------------------------------------------------------
	Form::~Form()
	{

	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Form::SetText(const Misc::UnicodeString &text)
	{
		textHelper.SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& Form::GetText() const
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
	bool Form::ContainsPoint(const Drawing::Point &point) const
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void Form::Invalidate()
	{
		captionBar = Drawing::Rectangle(bounds.GetLeft() + 1, bounds.GetTop() + 1, bounds.GetWidth() - 23, 17);
		closeRect = Drawing::Rectangle(captionBar.GetRight(), bounds.GetTop() + 2, 17, 17);

		clientArea = Drawing::Rectangle(bounds.GetLeft() + 3, bounds.GetTop() + 20, bounds.GetWidth() - 6, bounds.GetHeight() - 23);

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	void Form::Show(const std::shared_ptr<Form> &instance)
	{
		this->instance = std::weak_ptr<Form>(instance);
	
		Application::formManager.RegisterForm(this->instance.lock());

		isVisible = true;
		isEnabled = true;
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
	
		Application::formManager.RegisterForm(this->instance.lock(), closeFunction);

		isVisible = true;
		isEnabled = true;
	}
	//---------------------------------------------------------------------------
	void Form::Close()
	{
		Application::formManager.UnregisterForm(instance.lock());
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes Form::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}

		if (!isVisible || !isEnabled)
		{
			return IEvent::Continue;
		}

		static Drawing::Point oldMousePosition;
		Drawing::Point mousePositionBackup;

		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			if (Drawing::Rectangle(1, 1, captionBar.GetWidth(), captionBar.GetHeight()).Contains(mouse->Position) || drag) //CaptionBar
			{
				if (mouse->State == MouseEvent::Move && drag == true)
				{
					Drawing::Point delta = mousePositionBackup - oldMousePosition;
					oldMousePosition = mousePositionBackup;
					SetLocation(delta + GetLocation());

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftDown)
				{
					oldMousePosition = mousePositionBackup;
					drag = true;

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					drag = false;

					return IEvent::DontContinue;
				}
			}
			else if (Drawing::Rectangle(captionBar.GetWidth() + 1, 2, closeRect.GetWidth(), closeRect.GetHeight()).Contains(mouse->Position)) //closeRect
			{
				if (mouse->State == MouseEvent::LeftUp || mouse->State == MouseEvent::LeftDown)
				{
					static bool pressed = mouse->State == MouseEvent::LeftDown;

					if (pressed && mouse->State == MouseEvent::LeftUp)
					{
						Close();
					}
					
					return IEvent::DontContinue;
				}
			}

			mouse->Position -= clientArea.GetPosition() - bounds.GetPosition();
		}
	
		if (ChildProcessEvent(event) == IEvent::DontContinue)
		{
			return IEvent::DontContinue;
		}

		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;

			if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position))
			{
				if (mouse->State == MouseEvent::LeftDown || mouse->State == MouseEvent::RightDown)
				{
					MouseEventArgs args(mouse);
					mouseDownEvent.Invoke(this, args);

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::Move)
				{
					SetMouseOver(true);

					MouseEventArgs args(mouse);
					mouseMoveEvent.Invoke(this, args);
				}
				else if (mouse->State == MouseEvent::LeftUp || mouse->State == MouseEvent::RightUp)
				{
					clickEvent.Invoke(this);

					MouseEventArgs args(mouse);
					mouseClickEvent.Invoke(this, args);

					args = MouseEventArgs(mouse);
					mouseUpEvent.Invoke(this, args);
				}
			}
			
			if (mouse->State != MouseEvent::LeftDown && mouse->State != MouseEvent::RightDown)
			{
				return IEvent::DontContinue;
			}

			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == IEvent::Keyboard)
		{
			//swallow unhandled keyboard events
			return IEvent::DontContinue;
		}

		return IEvent::Continue;
	}
	//---------------------------------------------------------------------------
	void Form::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		renderer->SetRenderColor(backColor - Drawing::Color(0, 100, 100, 100));
		renderer->Fill(bounds);
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(bounds.GetLeft() + 1, bounds.GetTop() + 1, bounds.GetWidth() - 2, bounds.GetHeight() - 2, backColor - Drawing::Color(90, 90, 90));
		renderer->SetRenderColor(backColor - Drawing::Color(0, 50, 50, 50));
		renderer->Fill(bounds.GetLeft() + 5, captionBar.GetBottom() + 2, bounds.GetWidth() - 10, 1);

		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, captionBar.GetLeft() + 4, captionBar.GetTop() + 2, textHelper.GetText());

		for (int i = 0; i < 4; ++i)
		{
			renderer->Fill(closeRect.GetLeft() + 4 + i, closeRect.GetTop() + 4 + i, 3, 1);
			renderer->Fill(closeRect.GetLeft() + 10 - i, closeRect.GetTop() + 4 + i, 3, 1);
			renderer->Fill(closeRect.GetLeft() + 4 + i, closeRect.GetTop() + 11 - i, 3, 1);
			renderer->Fill(closeRect.GetLeft() + 10 - i, closeRect.GetTop() + 11 - i, 3, 1);
		}
		
		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea);

		ChildRender(renderer);
		
		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}