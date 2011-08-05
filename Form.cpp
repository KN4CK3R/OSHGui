#include "Form.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Form::Form() : Panel(this)
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
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Form::SetText(const Misc::UnicodeString &text)
	{
		this->text = text;
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& Form::GetText()
	{
		return text;
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
		captionBar = bounds;
		captionBar.Offset(1, 1);
		captionBar.Inflate(-32, 0);
		captionBar.SetHeight(17);
		
		closeRect = Drawing::Rectangle(bounds.GetRight() - 22, bounds.GetTop() + 2, 17, 17);
		minimizeRect = Drawing::Rectangle(bounds.GetRight() - 39, bounds.GetTop() + 2, 17, 17);
		
		clientArea = bounds;
		clientArea.Offset(3, 20);
		clientArea.Inflate(-6, -23);
	}
	//---------------------------------------------------------------------------
	void Form::Show()
	{
		visible = true;
		enabled = true;
	}
	//---------------------------------------------------------------------------
	Drawing::Point Form::PointToClient(const Drawing::Point &point)
	{
		return Drawing::Point(point.Left, point.Top - clientArea.GetTop());
	}
	//---------------------------------------------------------------------------
	Drawing::Point Form::PointToScreen(const Drawing::Point &point)
	{
		return Drawing::Point();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Form::ProcessEvent(Event *event)
	{
		static Drawing::Point oldMousePosition;
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
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
					Drawing::Point delta = mouse->Position - oldMousePosition + bounds.GetPosition();
					oldMousePosition = mouse->Position;
					this->SetLocation(delta);
				}
				else if (mouse->State == MouseEvent::LeftDown)
				{
					oldMousePosition = mouse->Position;
					drag = true;
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					drag = false;
				}
				return Event::DontContinue;
			}
			mouse->Position = PointToClient(mouse->Position);
		}
	
		return Panel::ProcessEvent(event);
	}
	//---------------------------------------------------------------------------
	void Form::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}

		Drawing::Point position = bounds.GetPosition();

		renderer->SetRenderColor(backColor - Drawing::Color(0, 100, 100, 100));
		renderer->Fill(bounds);
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(position.Left + 1, position.Top + 1, bounds.GetWidth() - 2, bounds.GetHeight() - 2, backColor - Drawing::Color(90, 90, 90));
		renderer->SetRenderColor(backColor - Drawing::Color(0, 50, 50, 50));
		renderer->Fill(position.Left + 5, captionBar.GetBottom() + 2, bounds.GetWidth() - 10, 1);
		//renderer->FillGradient(clientArea, backColor);

		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, captionBar.GetLeft() + 4, captionBar.GetTop() + 2, text);

		for (int i = 0; i < 4; i++)
		{
			renderer->Fill(closeRect.GetLeft() + 4 + i, closeRect.GetTop() + 4 + i, 3, 1);
			renderer->Fill(closeRect.GetLeft() + 10 - i, closeRect.GetTop() + 4 + i, 3, 1);
			renderer->Fill(closeRect.GetLeft() + 4 + i, closeRect.GetTop() + 11 - i, 3, 1);
			renderer->Fill(closeRect.GetLeft() + 10 - i, closeRect.GetTop() + 11 - i, 3, 1);
		}
		
		Drawing::Rectangle rect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea);
	
		for (unsigned int i = 0, len = Controls.size(); i < len; i++)
		{
			Controls.at(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(rect);
	}
	//---------------------------------------------------------------------------
}