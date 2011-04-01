#include "Form.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Form::Form()
	{
		type = CONTROL_FORM;
		
		visible = false;
		enabled = false;

		ParentPanel = this;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Form::SetText(LPCWSTR text)
	{
		if(text == NULL)
		{
			this->text[0] = 0;
			return;
		}

		wcscpy_s(this->text, 256, text);
	}
	//---------------------------------------------------------------------------
	LPCWSTR Form::GetText()
	{
		return text;
	}
	//---------------------------------------------------------------------------
	bool Form::GetTextCopy(LPWSTR copy)
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
	bool Form::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void Form::Show()
	{
		visible = true;
		enabled = true;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes Form::ProcessEvent(Event *event)
	{
		if (event->Type == Event::Mouse)
		{
			//if in caption
		}
	
		return Event::None;//Panel::ProcessEvent(event);
	}
	//---------------------------------------------------------------------------
	void Form::Render(Drawing::IRenderer *renderer)
	{
		/*Drawing::Size size(220, 224);

		texture2 = new Drawing::Texture(pDevice);
		texture2->Create(size);
		texture2->BeginUpdate();

		texture2->FillGradient(0xFF5A5655, 0xFF383735);

		texture2->EndUpdate();*/
		
		//captionbar
	
		for (int i = 0, len = Controls.GetSize(); i < len; i++)
		{
			Controls.Get(i)->Render(renderer);
		}
	}
	//---------------------------------------------------------------------------
}