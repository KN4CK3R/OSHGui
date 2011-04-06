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
	void GroupBox::SetText(LPCSTR text)
	{
		if(text == NULL)
		{
			this->text[0] = 0;
			return;
		}

		strcpy_s(this->text, 256, text);
	}
	//---------------------------------------------------------------------------
	LPCSTR GroupBox::GetText()
	{
		return text;
	}
	//---------------------------------------------------------------------------
	bool GroupBox::GetTextCopy(LPSTR copy)
	{
		if (copy == NULL)
		{
			return false;
		}
		
		strcpy_s(copy, wcslen(text), text);
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
		
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture, bounds.GetPosition());
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, textRect, text);
		
		Drawing::Rectangle rect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(bounds);
	
		for (int i = 0, len = Controls.GetSize(); i < len; i++)
		{
			Controls.Get(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(bounds);
	}
	//---------------------------------------------------------------------------
}