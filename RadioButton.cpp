#include "RadioButton.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	RadioButton::RadioButton(Panel *parentPanel)
	{
		type = CONTROL_RADIOBUTTON;
		
		ParentPanel = parentPanel;
		
		group = 0;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void RadioButton::SetChecked(bool checked)
	{
		if (this->checked != checked)
		{
			ParentPanel->ClearRadioButtonGroup(group);
			
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
		if (needRepaint)
		{
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture());
			}

			Drawing::Color color[3] = { Drawing::Color(0xFFAAA5A2), Drawing::Color(0xBFAAA5A2), Drawing::Color(0x80AAA5A2) };
			
			Drawing::ITexture *main = texture.Get(0);
			
			main->Create(Drawing::Size(15, 15));
			main->BeginUpdate();
			main->Clear();
			
			for (int i = 0; i < 2; i++)
			{
				main->Fill(1 - i, 7, 1, 1, color[(2 + i) % 3]);
				main->Fill(7, 1 - i, 1, 1, color[(2 + i) % 3]);
				main->Fill(13 + i, 7, 1, 1, color[(2 + i) % 3]);
				main->Fill(7, 13 + i, 1, 1, color[(2 + i) % 3]);
			}
			
			Drawing::ITexture *temp = renderer->CreateNewTexture();
			{
				Drawing::Size tempSize(7, 7);
				
				temp->Create(tempSize);
				temp->BeginUpdate();

				temp->Clear();
				
				for (int i = 0; i < 3; i++)
				{
					temp->Fill(6 - i, 0, 1, 1, color[i]);
					temp->Fill(0, 6 - i, 1, 1, color[i]);
				}
				for (int i = 0; i < 5; i++)
				{
					temp->Fill(2 + i, 1, 1, 1, color[(2 + i) % 3]);
					temp->Fill(1, 2 + i, 1, 1, color[(2 + i) % 3]);
				}
				temp->Fill(1, 6, 1, 1, Drawing::Color(0x3FAAA5A2));
				temp->Fill(6, 1, 1, 1, Drawing::Color(0x3FAAA5A2));
				temp->Fill(2, 2, 1, 1, color[1]);
				temp->Fill(2, 3, 1, 1, color[2]);
				temp->Fill(3, 2, 1, 1, color[2]);
				
				temp->EndUpdate();
			}
			
			main->Insert(0, 0, temp);
			temp->BeginUpdate();
			temp->Rotate(90);
			temp->EndUpdate();
			main->Insert(8, 0, temp);
			temp->BeginUpdate();
			temp->Rotate(90);
			temp->EndUpdate();
			main->Insert(8, 8, temp);
			temp->BeginUpdate();
			temp->Rotate(90);
			temp->EndUpdate();
			main->Insert(0, 8, temp);

			delete temp;

			if (checked)
			{
				color[0] = Drawing::Color::White();
				color[1] = Drawing::Color(0xBFFFFFFF);
				color[2] = Drawing::Color(0x80FFFFFF);
				
				main->Fill(5, 5, 5, 5, color[0]);
				
				for (int i = 0; i < 5; i++)
				{
					main->Fill(5 + i, 4, 1, 1, color[i < 2 ? 2 - i : -2 + i]);
					main->Fill(5 + i, 10, 1, 1, color[i < 2 ? 2 - i : -2 + i]);
					main->Fill(4, 5 + i, 1, 1, color[i < 2 ? 2 - i : -2 + i]);
					main->Fill(10, 5 + i, 1, 1, color[i < 2 ? 2 - i : -2 + i]);
				}
			}

			main->EndUpdate();
		}
		
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture.Get(0), buttonRect.GetPosition());
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, textRect, text);
	}
	//---------------------------------------------------------------------------
}