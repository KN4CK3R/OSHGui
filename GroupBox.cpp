#include "GroupBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	GroupBox::GroupBox(Control *parent) : Control(parent), textHelper(font)
	{
		type = CONTROL_GROUPBOX;

		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color::White());
	}
	//---------------------------------------------------------------------------
	GroupBox::~GroupBox()
	{
		for (unsigned int i = 0, len = controls.size(); i < len; i++)
		{
			Control *control = controls.at(i);
			delete control;
		}

		controls.clear();
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
		renderBounds = Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight());
		clientArea = Drawing::Rectangle(3, 10, bounds.GetWidth() - 6, bounds.GetHeight() - 13);

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes GroupBox::ProcessEvent(Event *event)
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

			mouse->Position.Top -= clientArea.GetTop();
		}
	
		if (ProcessChildrenEvent(event) == Event::DontContinue)
		{
			return Event::DontContinue;
		}

		return Event::DontContinue;
	}
	//---------------------------------------------------------------------------
	void GroupBox::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}

		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(bounds + renderRect.GetPosition());

		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(renderBounds);
		}
		
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, 5, -1, textHelper.GetText());

		renderer->Fill(1, 5, 3, 1);
		renderer->Fill(textHelper.GetSize().Width + 5, 5, renderBounds.GetWidth() - textHelper.GetSize().Width - 5, 1);
		renderer->Fill(0, 6, 1, renderBounds.GetHeight() - 7);
		renderer->Fill(renderBounds.GetWidth() - 1, 6, 1, renderBounds.GetHeight() - 7);
		renderer->Fill(1, renderBounds.GetHeight() - 1, renderBounds.GetWidth() - 2, 1);
		
		renderer->SetRenderRectangle(clientArea + bounds.GetPosition() + renderRect.GetPosition());
	
		for (unsigned int i = 0, len = controls.size(); i < len; i++)
		{
			controls.at(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}