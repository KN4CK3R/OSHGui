#include "GroupBox.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	GroupBox::GroupBox(Control *parent) : Control(parent), textHelper(font)
	{
		type = CONTROL_GROUPBOX;

		SetBounds(6, 6, 150, 150);
		
		SetText(L"GroupBox");

		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void GroupBox::SetText(const Misc::UnicodeString &text)
	{
		textHelper.SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& GroupBox::GetText() const
	{
		return textHelper.GetText();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool GroupBox::ContainsPoint(const Drawing::Point &point) const
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void GroupBox::Invalidate()
	{
		clientArea = Drawing::Rectangle(bounds.GetLeft() + 3, bounds.GetTop() + 10, bounds.GetWidth() - 6, bounds.GetHeight() - 13);

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes GroupBox::ProcessEvent(IEvent *event)
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
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			mouse->Position.Top -= (clientArea.GetTop() - bounds.GetTop());
		}
	
		if (ChildProcessEvent(event) == IEvent::DontContinue)
		{
			return IEvent::DontContinue;
		}

		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			mouse->Position = mousePositionBackup;

			if (Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight()).Contains(mouse->Position))
			{
				if (mouse->State == MouseEvent::LeftDown || mouse->State == MouseEvent::RightDown)
				{
					mouseDownEvent.Invoke(this, MouseEventArgs(mouse));

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::Move)
				{
					mouseMoveEvent.Invoke(this, MouseEventArgs(mouse));

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftUp || mouse->State == MouseEvent::RightUp)
				{
					mouseUpEvent.Invoke(this, MouseEventArgs(mouse));

					return IEvent::DontContinue;
				}
			}
		}

		return IEvent::Continue;
	}
	//---------------------------------------------------------------------------
	void GroupBox::Render(Drawing::IRenderer *renderer)
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
		renderer->RenderText(font, bounds.GetLeft() + 5, bounds.GetTop() - 1, textHelper.GetText());

		renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop() + 5, 3, 1);
		renderer->Fill(bounds.GetLeft() + textHelper.GetSize().Width + 5, bounds.GetTop() + 5, bounds.GetWidth() - textHelper.GetSize().Width - 6, 1);
		renderer->Fill(bounds.GetLeft(), bounds.GetTop() + 6, 1, bounds.GetHeight() - 7);
		renderer->Fill(bounds.GetLeft() + bounds.GetWidth() - 1, bounds.GetTop() + 6, 1, bounds.GetHeight() - 7);
		renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop() + bounds.GetHeight() - 1, bounds.GetWidth() - 2, 1);
		
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