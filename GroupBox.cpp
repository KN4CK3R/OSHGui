#include "GroupBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	GroupBox::GroupBox(const std::shared_ptr<Control> &parent) : Control(parent), textHelper(font)
	{
		type = CONTROL_GROUPBOX;

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
	const Misc::UnicodeString& GroupBox::GetText()
	{
		return textHelper.GetText();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool GroupBox::ContainsPoint(const Drawing::Point &point)
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
	Event::NextEventTypes GroupBox::ProcessEvent(const std::shared_ptr<Event> &event)
	{
		if (event == 0)
		{
			return Event::DontContinue;
		}

		if (!visible || !enabled)
		{
			return Event::Continue;
		}

		if (event->Type == Event::Mouse)
		{
			std::shared_ptr<MouseEvent> mouse = std::static_pointer_cast<MouseEvent>(event);
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			mouse->Position.Top -= (clientArea.GetTop() - bounds.GetTop());
		}
	
		if (ProcessChildrenEvent(event) == Event::DontContinue)
		{
			return Event::DontContinue;
		}

		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void GroupBox::Render(const std::shared_ptr<Drawing::IRenderer> &renderer)
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
				controls.at(i)->Render(renderer);
			}
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}