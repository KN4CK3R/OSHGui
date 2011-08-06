#include "Form.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Form::Form() : Control(this), textHelper(font)
	{
		type = CONTROL_FORM;
		
		visible = false;
		enabled = false;
		drag = false;

		SetLocation(Drawing::Point(10, 10));
		SetSize(Drawing::Size(364, 379));

		SetBackColor(Drawing::Color(0xFF7c7b79));
		SetForeColor(Drawing::Color::White());

		Invalidate();
	}
	//---------------------------------------------------------------------------
	Form::~Form()
	{
		for (unsigned int i = 0, len = controls.size(); i < len; i++)
		{
			Control *control = controls.at(i);
			delete control;
		}

		controls.clear();

		Control::~Control();
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
		captionBar = Drawing::Rectangle(1, 1, bounds.GetWidth() - 40, 17);
		
		closeRect = Drawing::Rectangle(bounds.GetWidth() - 22, 2, 17, 17);
		minimizeRect = Drawing::Rectangle(bounds.GetWidth() - 39, 2, 17, 17);
		
		clientArea = Drawing::Rectangle(3, 20, bounds.GetWidth() - 6, bounds.GetHeight() - 23);

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	void Form::Show()
	{
		visible = true;
		enabled = true;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Form::ProcessEvent(Event *event)
	{
		if (event == NULL)
		{
			return Event::DontContinue;
		}

		if (!visible || !enabled)
		{
			return Event::Continue;
		}

		static Drawing::Point oldMousePosition;
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			if (captionBar.Contains(mouse->Position) || drag)
			{
				if (mouse->State == MouseEvent::LeftUp)
				{
					if (closeRect.Contains(mouse->Position))
					{
						//close
					
						return Event::DontContinue;
					}
					else if (minimizeRect.Contains(mouse->Position))
					{
						//minimize
					
						return Event::DontContinue;
					}
				}

				if (mouse->State == MouseEvent::Move && drag == true)
				{
					Drawing::Point delta = mousePositionBackup - oldMousePosition;
					oldMousePosition = mousePositionBackup;
					SetLocation(delta + GetLocation());
				}
				else if (mouse->State == MouseEvent::LeftDown)
				{
					oldMousePosition = mousePositionBackup;
					drag = true;
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					drag = false;
				}
				return Event::DontContinue;
			}

			mouse->Position.Top -= captionBar.GetHeight();
		}
	
		if (ProcessChildrenEvent(event) == Event::DontContinue)
		{
			return Event::DontContinue;
		}

		return Event::DontContinue;
	}
	//---------------------------------------------------------------------------
	void Form::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}

		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(bounds);

		renderer->SetRenderColor(backColor - Drawing::Color(0, 100, 100, 100));
		renderer->Fill(0, 0, bounds.GetWidth(), bounds.GetHeight());
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(1, 1, bounds.GetWidth() - 2, bounds.GetHeight() - 2, backColor - Drawing::Color(90, 90, 90));
		renderer->SetRenderColor(backColor - Drawing::Color(0, 50, 50, 50));
		renderer->Fill(5, captionBar.GetBottom() + 2, bounds.GetWidth() - 10, 1);
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
		
		renderer->SetRenderRectangle(clientArea + bounds.GetPosition());
	
		for (unsigned int i = 0, len = controls.size(); i < len; i++)
		{
			controls.at(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}