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
	void Form::UpdateRects()
	{
		captionBar = bounds;
		captionBar.Offset(1, 1);
		captionBar.Inflate(-32, 0);
		captionBar.SetHeight(24);
		
		closeRect = Drawing::Rectangle(size.GetWidth() - 15, 7, 10, 10);
		minimizeRect = Drawing::Rectangle(size.GetWidth() - 32, 7, 10, 10);
		
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
			if (captionBar.IsIn(mouse->Position))
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
				return NextEventType::None;
			}
			if (mouse->State == MouseEvent::LeftUp)
			{
				if (closeRect.IsIn(mouse->Position))
				{
					//close
					
					return NextEventType::None;
				}
				else if (minimizeRect.IsIn(mouse->Position))
				{
					//minimize
					
					return NextEventType::None;
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
		
		renderer->SetRenderColor();
		renderer->RenderTexture(texture, bounds.GetPosition());
		renderer->SetRenderColor();
		renderer->RenderText(font, captionbar, text);
		
		Drawing::Rectangle rect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientRect);
	
		for (int i = 0, len = Controls.GetSize(); i < len; i++)
		{
			Controls.Get(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(rect);
	}
	//---------------------------------------------------------------------------
}