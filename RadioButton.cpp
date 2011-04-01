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
			if (texture == NULL)
			{
				texture = renderer->CreateNewTexture();
			}

			D3DCOLOR color[3] = { 0xFFAAA5A2, 0xBFAAA5A2, 0x80AAA5A2 };
			
			texture->Create(Drawing::Size(15, 15));
			texture->BeginUpdate();
			texture->Clear();
			
			for (int i = 0; i < 2; i++)
			{
				texture->Fill(1 - i, 7, 1, 1, color[(2 + i) % 3]);
				texture->Fill(7, 1 - i, 1, 1, color[(2 + i) % 3]);
				texture->Fill(13 + i, 7, 1, 1, color[(2 + i) % 3]);
				texture->Fill(7, 13 + i, 1, 1, color[(2 + i) % 3]);
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
				temp->Fill(1, 6, 1, 1, 0x3FAAA5A2);
				temp->Fill(6, 1, 1, 1, 0x3FAAA5A2);
				temp->Fill(2, 2, 1, 1, color[1]);
				temp->Fill(2, 3, 1, 1, color[2]);
				temp->Fill(3, 2, 1, 1, color[2]);
				
				temp->EndUpdate();
			}
			
			texture->Insert(0, 0, temp);
			temp->BeginUpdate();
			temp->Rotate(90);
			temp->EndUpdate();
			texture->Insert(8, 0, temp);
			temp->BeginUpdate();
			temp->Rotate(90);
			temp->EndUpdate();
			texture->Insert(8, 8, temp);
			temp->BeginUpdate();
			temp->Rotate(90);
			temp->EndUpdate();
			texture->Insert(0, 8, temp);

			delete temp;

			if (checked)
			{
				color[0] = 0xFFFFFFFF;
				color[1] = 0xBFFFFFFF;
				color[2] = 0x80FFFFFF;
				
				texture->Fill(5, 5, 5, 5, color[0]);
				
				for (int i = 0; i < 5; i++)
				{
					texture->Fill(5 + i, 4, 1, 1, color[i < 2 ? 2 - i : -2 + i]);
					texture->Fill(5 + i, 10, 1, 1, color[i < 2 ? 2 - i : -2 + i]);
					texture->Fill(4, 5 + i, 1, 1, color[i < 2 ? 2 - i : -2 + i]);
					texture->Fill(10, 5 + i, 1, 1, color[i < 2 ? 2 - i : -2 + i]);
				}
			}

			texture->EndUpdate();
		}
		
		renderer->SetRenderColor();
		renderer->RenderTexture(texture, buttonRect.GetPosition());
		renderer->SetRenderColor();
		renderer->RenderText(font, textRect, text);
	}
	//---------------------------------------------------------------------------
}