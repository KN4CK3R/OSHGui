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
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Label::SetText(LPCSTR text)
	{
		if(text == NULL)
		{
			this->text[0] = 0;
			return;
		}

		strcpy_s(this->text, 256, text);
	}
	//---------------------------------------------------------------------------
	LPCSTR Label::GetText()
	{
		return text;
	}
	//---------------------------------------------------------------------------
	bool Label::GetTextCopy(LPSTR copy)
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
		return Event::None;
	}
	//---------------------------------------------------------------------------
	void Label::Render(Drawing::IRenderer *renderer)
	{
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