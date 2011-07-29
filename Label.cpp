#include "Label.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Label::Label(Panel *parentPanel)
	{
		type = CONTROL_LABEL;
		
		ParentPanel = parentPanel;
		
		memset((void*)&text, 0x00, sizeof(text));
		
		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFFA3A3A3));
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Label::SetText(const char *text)
	{
		if(text == NULL)
		{
			this->text[0] = 0;
			return;
		}

		strcpy_s(this->text, 256, text);
	}
	//---------------------------------------------------------------------------
	const char* Label::GetText()
	{
		return text;
	}
	//---------------------------------------------------------------------------
	bool Label::GetTextCopy(char *copy)
	{
		if (copy == NULL)
		{
			return false;
		}
		
		strcpy_s(copy, strlen(text), text);
		return true;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Label::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Label::ProcessEvent(Event *event)
	{
		if (event == NULL || !visible || !enabled)
		{
			return Event::None;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*) event;
			if (mouse->State == MouseEvent::LeftDown)
			{
				pressed = true;
			}
			if (mouse->State == MouseEvent::LeftUp)
			{
				if (pressed && hasFocus)
				{
					pressed = false;
					
					if (clickFunc != NULL)
					{
						(*clickFunc)(this, mouse);
					}
				}
			}
		}
		
		return Event::None;
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
		renderer->RenderText(font, bounds, text);
	}
	//---------------------------------------------------------------------------
}