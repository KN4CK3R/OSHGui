#include "CheckBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	CheckBox::CheckBox(Control *parent) : Label(parent)
	{
		type = CONTROL_CHECKBOX;
		
		SetText(L"CheckBox");

		checked = false;

		SetBackColor(Drawing::Color(0xFF222222));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void CheckBox::SetChecked(bool checked)
	{
		if (this->checked != checked)
		{
			this->checked = checked;
			
			changeEventHandler.Invoke(this);
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
			size.Inflate(20, 4);
			if (size.Height < 17)
			{
				size.Height = 17;
			}
			SetSize(size);
		}
		clientArea = bounds;

		if (font->GetSize() < 17)
		{
			checkBoxPosition = bounds.GetPosition();
			int y = (int)(17 / 2.0f - font->GetSize() / 2.0f + 0.5f);
			textPosition = Drawing::Point(bounds.GetLeft() + 20, bounds.GetTop() + y);
		}
		else
		{
			textPosition = bounds.GetPosition().OffsetEx(20, 0);
			int y = (int)(font->GetSize() / 2.0f - 17 / 2.0f + 0.5f);
			checkBoxPosition = Drawing::Point(bounds.GetLeft(), bounds.GetTop() + y);
		}

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes CheckBox::ProcessEvent(Event *event)
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
			MouseEvent *mouse = (MouseEvent*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
			
			if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position)) //ClientArea
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
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(checkBoxPosition.Left, checkBoxPosition.Top, 17, 17);

		Drawing::Color white = Drawing::Color::White();
		renderer->SetRenderColor(white);
		renderer->FillGradient(checkBoxPosition.Left + 1, checkBoxPosition.Top + 1, 15, 15, white - Drawing::Color(0, 137, 137, 137));
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(checkBoxPosition.Left + 2, checkBoxPosition.Top + 2, 13, 13, backColor + Drawing::Color(0, 55, 55, 55));
		
		renderer->SetRenderColor(white);		
		if (checked)
		{
			renderer->Fill(checkBoxPosition.Left + 5, checkBoxPosition.Top + 5, 7, 7);
			renderer->FillGradient(checkBoxPosition.Left + 6, checkBoxPosition.Top + 6, 5, 5, white - Drawing::Color(0, 137, 137, 137));
		}
		
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, textPosition.Left, textPosition.Top, bounds.GetWidth() - 20, bounds.GetHeight(), textHelper.GetText());

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