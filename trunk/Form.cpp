#include "Form.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Form::Form() : Control(0), textHelper(font)
	{
		type = CONTROL_FORM;
		
		visible = false;
		enabled = false;
		drag = false;

		SetLocation(Drawing::Point(10, 10));
		SetSize(Drawing::Size(300, 300));

		SetBackColor(Drawing::Color(0xFF7c7b79));
		SetForeColor(Drawing::Color(0xFFE5E0E4));

		Invalidate();
	}
	//---------------------------------------------------------------------------
	Form::~Form()
	{

	}
	//---------------------------------------------------------------------------
	void Form::Dispose()
	{
		controls.clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Form::SetText(const Misc::UnicodeString &text)
	{
		textHelper.SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& Form::GetText()
	{
		return textHelper.GetText();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Form::ContainsPoint(const Drawing::Point &point)
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
	void Form::Show()
	{
		Application::RegisterForm(std::static_pointer_cast<Form>(shared_from_this()));

		visible = true;
		enabled = true;
	}
	//---------------------------------------------------------------------------
	void Form::Close()
	{
		Application::UnregisterForm(std::static_pointer_cast<Form>(shared_from_this()));
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Form::ProcessEvent(const std::shared_ptr<Event> &event)
	{
		if (event == 0)
		{
			return Event::DontContinue;
		}

		if (!visible || !enabled)
		{
			return Event::Continue;
		}

		static Drawing::Point oldMousePosition;
		Drawing::Point mousePositionBackup;

		if (event->Type == Event::Mouse)
		{
			std::shared_ptr<MouseEvent> mouse = std::static_pointer_cast<MouseEvent>(event);
			mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			if (Drawing::Rectangle(1, 1, captionBar.GetWidth(), captionBar.GetHeight()).Contains(mouse->Position) || drag) //CaptionBar
			{
				if (mouse->State == MouseEvent::Move && drag == true)
				{
					Drawing::Point delta = mousePositionBackup - oldMousePosition;
					oldMousePosition = mousePositionBackup;
					SetLocation(delta + GetLocation());

					return Event::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftDown)
				{
					oldMousePosition = mousePositionBackup;
					drag = true;

					return Event::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					drag = false;

					return Event::DontContinue;
				}
			}
			else if (mouse->State == MouseEvent::LeftUp || mouse->State == MouseEvent::LeftDown)
			{
				if (Drawing::Rectangle(captionBar.GetWidth() + 1, 2, closeRect.GetWidth(), closeRect.GetHeight()).Contains(mouse->Position)) //coseRect
				{
					static bool pressed = mouse->State == MouseEvent::LeftDown;

					if (pressed && mouse->State == MouseEvent::LeftUp)
					{
						Close();
					}
					
					return Event::DontContinue;
				}
			}

			mouse->Position.Top -= captionBar.GetHeight();
		}
	
		if (ProcessChildrenEvent(event) == Event::DontContinue)
		{
			return Event::DontContinue;
		}

		if (event->Type == Event::Mouse)
		{
			std::shared_ptr<MouseEvent> mouse = std::static_pointer_cast<MouseEvent>(event);

			if (mouse->State != MouseEvent::LeftDown && mouse->State != MouseEvent::RightDown)
			{
				return Event::DontContinue;
			}
			else
			{
				if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position))
				{
					return Event::DontContinue;
				}
			}

			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == Event::Keyboard)
		{
			//swallow unhandled keyboard events
			return Event::DontContinue;
		}

		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void Form::Render(const std::shared_ptr<Drawing::IRenderer> &renderer)
	{
		if (!visible)
		{
			return;
		}

		renderer->SetRenderColor(backColor - Drawing::Color(0, 100, 100, 100));
		renderer->Fill(bounds);
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(bounds.GetLeft() + 1, bounds.GetTop() + 1, bounds.GetWidth() - 2, bounds.GetHeight() - 2, backColor - Drawing::Color(90, 90, 90));
		renderer->SetRenderColor(backColor - Drawing::Color(0, 50, 50, 50));
		renderer->Fill(bounds.GetLeft() + 5, captionBar.GetBottom() + 2, bounds.GetWidth() - 10, 1);
		//renderer->FillGradient(clientArea, backColor);

		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, captionBar.GetLeft() + 4, captionBar.GetTop() + 2, textHelper.GetText());

		for (int i = 0; i < 4; i++)
		{
			renderer->Fill(closeRect.GetLeft() + 4 + i, closeRect.GetTop() + 4 + i, 3, 1);
			renderer->Fill(closeRect.GetLeft() + 10 - i, closeRect.GetTop() + 4 + i, 3, 1);
			renderer->Fill(closeRect.GetLeft() + 4 + i, closeRect.GetTop() + 11 - i, 3, 1);
			renderer->Fill(closeRect.GetLeft() + 10 - i, closeRect.GetTop() + 11 - i, 3, 1);
		}
		
		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea);

		for (unsigned int i = 0; i < controls.size(); ++i)
		{
			controls.at(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}