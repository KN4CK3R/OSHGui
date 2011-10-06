#include "Label.h"
#include "..\Misc\Exceptions.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Label::Label(Control *parent) : Control(parent), textHelper(font)
	{
		type = CONTROL_LABEL;
		
		SetAutoSize(true);
		
		SetLocation(6, 6);

		SetText(L"Label");
		
		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFFE5E0E4));
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
	bool Label::ContainsPoint(const Drawing::Point &point) const
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void Label::Invalidate()
	{
		textHelper.SetFont(font);
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
	IEvent::NextEventTypes Label::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}

		if (!visible || !enabled)
		{
			return IEvent::Continue;
		}
	
		Drawing::Point mousePositionBackup;
		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
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
					pressed = true;
					
					mouseDownEvent.Invoke(this, MouseEventArgs(mouse));

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::Move)
				{
					mouseMoveEvent.Invoke(this, MouseEventArgs(mouse));
				}
				else if (mouse->State == MouseEvent::LeftUp || mouse->State == MouseEvent::RightUp)
				{
					if (pressed)
					{
						pressed = false;
					
						clickEvent.Invoke(this);

						mouseClickEvent.Invoke(this, MouseEventArgs(mouse));
					}

					mouseUpEvent.Invoke(this, MouseEventArgs(mouse));

					return IEvent::DontContinue;
				}
			}

			mouse->Position = mousePositionBackup;
		}
		
		return IEvent::Continue;
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
				controls[i]->Render(renderer);
			}
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}