#include "Label.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Label::Label(Control *parent) : Control(parent)
	{
		type = CONTROL_LABEL;
		
		autoSize = true;
		SetSize(Drawing::Size(10, OSHGui::GetDefaultFont().GetSize()));
		
		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color(0xFFA3A3A3));
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Label::SetText(const Misc::UnicodeString &text)
	{
		this->text = text;
		Invalidate();
	}
	//---------------------------------------------------------------------------
	Misc::UnicodeString& Label::GetText()
	{
		return text;
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
			SetSize(font->MeasureText(text));
		}
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
			MouseEvent *mouse = (MouseEvent*)event;
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