#include "GroupBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	GroupBox::GroupBox(Control *parent) : Control(parent)
	{
		type = CONTROL_GROUPBOX;
	}
	//---------------------------------------------------------------------------
	GroupBox::~GroupBox()
	{
		for (unsigned int i = 0, len = controls.size(); i < len; i++)
		{
			Control *control = controls.at(i);
			delete control;
		}

		controls.clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void GroupBox::SetText(const Misc::UnicodeString &text)
	{
		this->text = text;
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& GroupBox::GetText()
	{
		return text;
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
		//todo
		return Event::DontContinue;
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
	
		for (unsigned int i = 0, len = controls.size(); i < len; i++)
		{
			controls.at(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(bounds);
	}
	//---------------------------------------------------------------------------
}