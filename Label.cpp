#include "Label.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Label::Label(Control *parent) : Control(parent), textHelper(font)
	{
		type = CONTROL_LABEL;
		
		SetAutoSize(true);
		
		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFFA3A3A3));
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Label::SetText(const Misc::UnicodeString &text)
	{
		textHelper.SetText(text);
		Invalidate();
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& Label::GetText()
	{
		return textHelper.GetText();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Label::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void Label::Invalidate()
	{
		if (autoSize)
		{
			SetSize(textHelper.GetSize());
		}

		clientArea = bounds;

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Label::ProcessEvent(Event *event)
	{
		if (event == NULL)
		{
			return Event::DontContinue;
		}

		if (!visible || !enabled)
		{
			return Event::Continue;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position))
			{
				if (mouse->State == MouseEvent::LeftDown)
				{
					pressed = true;

					return Event::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					if (pressed && hasFocus)
					{
						pressed = false;
					
						if (clickFunc != NULL)
						{
							(*clickFunc)(this, mouse);
						}
					}

					return Event::DontContinue;
				}
			}

			mouse->Position = mousePositionBackup;
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void Label::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
			
		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(bounds);
		}
	
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, bounds, textHelper.GetText());

		if (controls.size() > 0)
		{
			Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
			renderer->SetRenderRectangle(clientArea + renderRect.GetPosition());
			
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				controls.at(i)->Render(renderer);
			}
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}