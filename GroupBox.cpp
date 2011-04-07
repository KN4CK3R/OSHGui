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
	GroupBox::~GroupBox()
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
		
		strcpy_s(copy, strlen(text), text);
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
	void GroupBox::Render(Drawing::IRenderer *renderer)
	{
		if (needsRepaint)
		{
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture());
			}
	
			Drawing::Size size = bounds.GetSize();
			
			Drawing::ITexture *main = texture.Get(0);

			main->Create(size);
			main->BeginUpdate();

			main->FillGradient(Color(0xFF5A5655), Color(0xFF383735));

			main->EndUpdate();
		}
		
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture.Get(0), bounds.GetPosition());
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