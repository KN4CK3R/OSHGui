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
		drag = false;

		ParentPanel = this;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Form::SetText(LPCSTR text)
	{
		if(text == NULL)
		{
			this->text[0] = 0;
			return;
		}

		strcpy_s(this->text, 256, text);
	}
	//---------------------------------------------------------------------------
	LPCSTR Form::GetText()
	{
		return text;
	}
	//---------------------------------------------------------------------------
	bool Form::GetTextCopy(LPSTR copy)
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
	bool Form::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void Form::UpdateRects()
	{
		captionBar = bounds;
		captionBar.Offset(1, 1);
		captionBar.Inflate(-32, 0);
		captionBar.SetHeight(24);
		
		closeRect = Drawing::Rectangle(bounds.GetWidth() - 15, 7, 10, 10);
		minimizeRect = Drawing::Rectangle(bounds.GetWidth() - 32, 7, 10, 10);
		
		clientArea = bounds;
		clientArea.Offset(1, 25);
		clientArea.Inflate(-2, -25);
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
		static Drawing::Point oldMousePosition;
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			if (captionBar.Contains(mouse->Position))
			{
				if (mouse->State == MouseEvent::Move && drag == true)
				{
					
				}
				else if (mouse->State == MouseEvent::LeftDown)
				{
					oldMousePosition = mouse->Position;
					drag = true;
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					drag = false;
				}
				return Event::None;
			}
			if (mouse->State == MouseEvent::LeftUp)
			{
				if (closeRect.Contains(mouse->Position))
				{
					//close
					
					return Event::None;
				}
				else if (minimizeRect.Contains(mouse->Position))
				{
					//minimize
					
					return Event::None;
				}
			}
		}
	
		return Panel::ProcessEvent(event);
	}
	//---------------------------------------------------------------------------
	void Form::Render(Drawing::IRenderer *renderer)
	{
		if (needsRepaint)
		{
			if (texture == NULL)
			{
				texture = renderer->CreateNewTexture();
			}
			Drawing::Size size = bounds.GetSize();
			texture->Create(size);
			texture->BeginUpdate();
			
			texture->Fill(0, 0, size.Width, 1, 0xFF000000);
			texture->Fill(0, 0, 1, size.Height, 0xFF000000);
			texture->Fill(size.Width - 1, 0, 1, size.Height, 0xFF000000);
			texture->Fill(0, size.Height - 1, size.Width, 1, 0xFF000000);
			
			//captionbar
			{
				texture->FillGradient(captionBar, 0xFF5F5A59, 0xFF444341);
				
				for (int i = 0; i < 4; i++)
				{
					texture->Fill(size.Width - 16 + i, 8 + i, 2, 1, 0xFFBAB9B7);
					texture->Fill(size.Width - 16 + i, 16 - i, 2, 1, 0xFFBAB9B7);
					texture->Fill(size.Width - 8 - i, 8 + i, 2, 1, 0xFFBAB9B7);
					texture->Fill(size.Width - 8 - i, 16 - i, 2, 1, 0xFFBAB9B7);
				}
			}
			
			//clientArea
			{
				texture->FillGradient(clientArea, 0xFF5A5655, 0xFF383735);
			}
			
			texture->EndUpdate();
		}
		
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture, bounds.GetPosition());
		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, captionBar, text);
		
		Drawing::Rectangle rect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea);
	
		for (int i = 0, len = Controls.GetSize(); i < len; i++)
		{
			Controls.Get(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(rect);
	}
	//---------------------------------------------------------------------------
}