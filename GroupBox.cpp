#include "GroupBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	GroupBox::GroupBox(Control *parent) : Panel(parent)
	{
		type = CONTROL_GROUPBOX;
	}
	//---------------------------------------------------------------------------
	GroupBox::~GroupBox()
	{
		for (unsigned int i = 0, len = Controls.size(); i < len; i++)
		{
			Control *control = Controls.at(i);
			delete control;
		}
	
		//Controls.Clear();
		Controls.clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void GroupBox::SetText(const char *text)
	{
		if(text == NULL)
		{
			this->text[0] = 0;
			return;
		}

		strcpy_s(this->text, 256, text);
	}
	//---------------------------------------------------------------------------
	const char* GroupBox::GetText()
	{
		return text;
	}
	//---------------------------------------------------------------------------
	bool GroupBox::GetTextCopy(char *copy)
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
	Event::NextEventTypes GroupBox::ProcessMessage(Event *event)
	{
		return Event::None;
	}
	//---------------------------------------------------------------------------
	void GroupBox::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
	
		if (needRepaint)
		{
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture());
			}
	
			Drawing::Size size = bounds.GetSize();
			
			Drawing::ITexture *main = texture.Get(0);

			main->Create(size);
			main->BeginUpdate();

			main->FillGradient(Drawing::Color(0xFF5A5655), Drawing::Color(0xFF383735));

			main->EndUpdate();
		}
		
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture.Get(0), bounds.GetPosition());
		renderer->SetRenderColor(foreColor);
		//renderer->RenderText(font, textRect, text);
		
		Drawing::Rectangle rect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(bounds);
	
		for (unsigned int i = 0, len = Controls.size(); i < len; i++)
		{
			Controls.at(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(bounds);
	}
	//---------------------------------------------------------------------------
}