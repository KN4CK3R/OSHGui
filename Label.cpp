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
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Label::SetText(LPCWSTR text)
	{
		if(text == NULL)
		{
			this->text[0] = 0;
			return;
		}

		wcscpy_s(this->text, 256, text);
	}
	//---------------------------------------------------------------------------
	LPCWSTR Label::GetText()
	{
		return text;
	}
	//---------------------------------------------------------------------------
	bool Label::GetTextCopy(LPWSTR copy)
	{
		if (copy == NULL)
		{
			return false;
		}
		
		wcscpy_s(copy, wcslen(text), text);
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
		renderer->RenderText(font, bounds, text);
	}
	//---------------------------------------------------------------------------
}