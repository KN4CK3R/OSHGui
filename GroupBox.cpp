#include "GroupBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	GroupBox::GroupBox(Panel *parentPanel)
	{
		type = CONTROL_GROUPBOX;
		
		ParentPanel = parentPanel;
		
		visible = false;
		enabled = false;
	}
	//---------------------------------------------------------------------------
	~GroupBox()
	{
		for (int i = 0, len = Controls.GetSize(); i < len; i++)
		{
			Control *control = Controls.Get(i);
			delete control;
		}
	
		Controls.Clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void GroupBox::SetText(LPCWSTR text)
	{
		if(text == NULL)
		{
			this->text[0] = 0;
			return;
		}

		wcscpy_s(this->text, 256, text);
	}
	//---------------------------------------------------------------------------
	LPCWSTR GroupBox::GetText()
	{
		return text;
	}
	//---------------------------------------------------------------------------
	bool GroupBox::GetTextCopy(LPWSTR copy)
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
	bool GroupBox::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void GroupBox::Render(IRenderer *renderer)
	{
		/*Drawing::Size size(220, 224);

		texture2 = new Drawing::Texture(pDevice);
		texture2->Create(size);
		texture2->BeginUpdate();

		texture2->FillGradient(0xFF5A5655, 0xFF383735);

		texture2->EndUpdate();*/
		
		//render text
	
		for (int i = 0, len = Controls.GetSize(); i < len; i++)
		{
			Controls.Get(i)->Render(renderer);
		}
	}
	//---------------------------------------------------------------------------
}