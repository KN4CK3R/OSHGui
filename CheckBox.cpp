#include "CheckBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	CheckBox::CheckBox(Control *parent) : Label(parent)
	{
		type = CONTROL_CHECKBOX;
				
		checked = false;

		SetBackColor(Drawing::Color(0xFF222222));
		SetForeColor(Drawing::Color::White());
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void CheckBox::SetChecked(bool checked)
	{
		if (this->checked != checked)
		{
			this->checked = checked;
			if (changeFunc != NULL)
			{
				(*changeFunc)(this);
			}
		}
	}
	//---------------------------------------------------------------------------
	bool CheckBox::GetChecked()
	{
		return checked;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool CheckBox::CanHaveFocus()
	{
		return visible && enabled;
	}
	//---------------------------------------------------------------------------
	void CheckBox::Invalidate()
	{
		if (autoSize)
		{
			Drawing::Size size = textHelper.GetSize();
			size.Width += 20;
			size.Height += 4;
			SetSize(size);
		}
		clientArea = Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight());
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes CheckBox::ProcessEvent(Event *event)
	{
		if (event == NULL || !visible || !enabled)
		{
			return Event::DontContinue;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			if (clientArea.Contains(mouse->Position))
			{			
				if (mouse->State == MouseEvent::LeftDown)
				{
					pressed = true;
			
					if (!hasFocus)
					{
						Parent->RequestFocus(this);
					}
					return Event::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					if (pressed && hasFocus)
					{
						SetChecked(!GetChecked());
					
						pressed = false;
					}
					return Event::DontContinue;
				}
			}

			//restore PointToClient (alternatively call PointToScreen)
			mouse->Position = mousePositionBackup;
		}
		else if (event->Type == Event::Keyboard)
		{
			KeyboardEvent *keyboard = (KeyboardEvent*)event;
			if (keyboard->KeyCode == Key::Space)
			{
				SetChecked(!GetChecked());
				return Event::DontContinue;
			}
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void CheckBox::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
		
		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea + bounds.GetPosition() + renderRect.GetPosition());
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(0, 0, 17, 17);
		renderer->SetRenderColor(foreColor);
		renderer->FillGradient(1, 1, 15, 15, foreColor - Drawing::Color(0, 137, 137, 137));
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(2, 2, 13, 13, backColor + Drawing::Color(0, 55, 55, 55));
		
		renderer->SetRenderColor(foreColor);
		
		if (checked)
		{
			renderer->Fill(5, 5, 7, 7);
			renderer->FillGradient(6, 6, 5, 5, foreColor - Drawing::Color(0, 137, 137, 137));
		}
		
		renderer->RenderText(font, 20, 2, clientArea.GetWidth() - 20, clientArea.GetHeight(), textHelper.GetText());

		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}