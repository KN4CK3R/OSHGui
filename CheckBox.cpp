#include "CheckBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	CheckBox::CheckBox(Control *parent) : Button(parent)
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
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes CheckBox::ProcessEvent(Event *event)
	{
		if (event == NULL || !visible || !enabled)
		{
			return Event::None;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			if (bounds.GetSize().Contains(mouse->Position))
			{			
				if (mouse->State == MouseEvent::LeftDown)
				{
					pressed = true;
			
					if (!hasFocus)
					{
						Parent->RequestFocus(this);
					}
					return Event::None;
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					if (pressed && hasFocus)
					{
						SetChecked(!GetChecked());
					
						pressed = false;
					}
					return Event::None;
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
				return Event::None;
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
		
		Drawing::Point position = bounds.GetPosition();
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(position.Left, position.Top, 17, 17);
		renderer->SetRenderColor(foreColor);
		renderer->FillGradient(position.Left + 1, position.Top + 1, 15, 15, foreColor - Drawing::Color(0, 137, 137, 137));
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(position.Left + 2, position.Top + 2, 13, 13, backColor + Drawing::Color(0, 55, 55, 55));
		
		renderer->SetRenderColor(foreColor);
		
		if (checked)
		{
			renderer->Fill(position.Left + 5, position.Top + 5, 7, 7);
			renderer->FillGradient(position.Left + 6, position.Top + 6, 5, 5, foreColor - Drawing::Color(0, 137, 137, 137));
		}
		
		renderer->RenderText(font, position.Left + 20, position.Top + 2, bounds.GetWidth() - 20, bounds.GetHeight(), text);
	}
	//---------------------------------------------------------------------------
}