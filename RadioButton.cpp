#include "RadioButton.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	RadioButton::RadioButton(Control *parent) : CheckBox(parent)
	{
		type = CONTROL_RADIOBUTTON;
				
		group = 0;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void RadioButton::SetChecked(bool checked)
	{
		if (this->checked != checked)
		{
			//uncheck other radiobuttons
			const std::vector<Control*> &controls = Parent->GetControls();
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				Control *control = controls.at(i);
				if (control->GetType() == CONTROL_RADIOBUTTON)
				{
					RadioButton *radio = (RadioButton*)control;
					if (radio->GetGroup() == group)
					{
						radio->SetCheckedInternal(false);
					}
				}
			}
			
			SetCheckedInternal(checked);
		}
	}
	//---------------------------------------------------------------------------
	void SetCheckedInternal(bool checked)
	{
		if (this->checked != checked)
		{			
			this->checked = checked;
			if (changeFunc != NULL)
			{
				(*changeFunc)(this);
			}
		}
	}
	//---------------------------------------------------------------------------
	void RadioButton::SetGroup(int group)
	{
		this->group = group;
	}
	//---------------------------------------------------------------------------
	int RadioButton::GetGroup()
	{
		return group;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void RadioButton::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
		
		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea + bounds.GetPosition() + renderRect.GetPosition());
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(0, 0, 17, 17);
		renderer->SetRenderColor(foreColor);
		renderer->FillGradient(1, 1, 15, 15, foreColor - Drawing::Color(0, 137, 137, 137));
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(2, 2, 13, 13, backColor + Drawing::Color(0, 55, 55, 55));
			
		if (checked)
		{
			renderer->Fill(6, 4, 5, 9);
			renderer->Fill(4, 6, 9, 5);
			renderer->Fill(5, 5, 7, 7);
			
			renderer->SetRenderColor(foreColor - Drawing::Color(0, 128, 128, 128));
			renderer->Fill(5, 7, 7, 3);
			
			renderer->SetRenderColor(foreColor);
			renderer->FillGradient(7, 5, 3, 7, foreColor - Drawing::Color(0, 137, 137, 137));
			renderer->FillGradient(6, 6, 5, 5, foreColor - Drawing::Color(0, 137, 137, 137));
		}
		
		renderer->RenderText(font, 20, 2, clientArea.GetWidth() - 20, clientArea.GetHeight(), textHelper.GetText());

		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}