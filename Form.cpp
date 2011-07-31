#include "Form.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Form::Form() : Panel(this)
	{
		type = CONTROL_FORM;
		
		visible = false;
		enabled = false;
		drag = false;

		memset((void*)&text, 0x00, sizeof(text));

		SetLocation(Drawing::Point(10, 10));
		SetSize(Drawing::Size(364, 379));

		SetBackColor(Drawing::Color(0xFF7c7b79));
		SetForeColor(Drawing::Color::White());

		UpdateRects();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Form::SetText(const char *text)
	{
		if(text == NULL)
		{
			this->text[0] = 0;
			return;
		}

		strcpy_s(this->text, 256, text);
		this->text[255] = 0;
	}
	//---------------------------------------------------------------------------
	const char* Form::GetText()
	{
		return text;
	}
	//---------------------------------------------------------------------------
	bool Form::GetTextCopy(char *copy)
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
		captionBar.SetHeight(17);
		
		closeRect = Drawing::Rectangle(bounds.GetRight() - 22, bounds.GetTop() + 2, 17, 17);
		minimizeRect = Drawing::Rectangle(bounds.GetRight() - 39, bounds.GetTop() + 2, 17, 17);
		
		clientArea = bounds;
		clientArea.Offset(3, 20);
		clientArea.Inflate(-6, -23);
	}
	//---------------------------------------------------------------------------
	void Form::Show()
	{
		visible = true;
		enabled = true;
	}
	//---------------------------------------------------------------------------
	Drawing::Point Form::PointToClient(const Drawing::Point &point)
	{
		return Drawing::Point(point.Left - clientArea.GetLeft(), point.Top - clientArea.GetTop());
	}
	//---------------------------------------------------------------------------
	Drawing::Point Form::PointToScreen(const Drawing::Point &point)
	{
		return Drawing::Point();
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
			if (captionBar.Contains(mouse->Position) || drag)
			{
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

				if (mouse->State == MouseEvent::Move && drag == true)
				{
					Drawing::Point delta = mouse->Position - oldMousePosition + bounds.GetPosition();
					oldMousePosition = mouse->Position;
					this->SetLocation(delta);
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
		}
	
		return Panel::ProcessEvent(event);
	}
	//---------------------------------------------------------------------------
	void Form::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
	
		/*if (needRepaint)
		{
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture());
			}
			
			Drawing::Size size = bounds.GetSize();
			
			Drawing::ITexture *main = texture.Get(0);
			
			main->Create(size);
			main->BeginUpdate();
			
			main->Fill(0, 0, size.Width, 1, Drawing::Color::Black());
			main->Fill(0, 0, 1, size.Height, Drawing::Color::Black());
			main->Fill(size.Width - 1, 0, 1, size.Height, Drawing::Color::Black());
			main->Fill(0, size.Height - 1, size.Width, 1, Drawing::Color::Black());
			
			//captionbar
			{
				main->FillGradient(captionBar, Drawing::Color(0xFF5F5A59), Drawing::Color(0xFF444341));
				
				Drawing::Color border(0xFFBAB9B7);
				for (int i = 0; i < 4; i++)
				{
					main->Fill(size.Width - 16 + i, 8 + i, 2, 1, border);
					main->Fill(size.Width - 16 + i, 16 - i, 2, 1, border);
					main->Fill(size.Width - 8 - i, 8 + i, 2, 1, border);
					main->Fill(size.Width - 8 - i, 16 - i, 2, 1, border);
				}
			}
			
			//clientArea
			{
				main->FillGradient(clientArea, Drawing::Color(0xFF5A5655), Drawing::Color(0xFF383735));
			}
			
			main->EndUpdate();

			needRepaint = false;
		}*/

		Drawing::Point position = bounds.GetPosition();

		renderer->SetRenderColor(backColor - Drawing::Color(0, 100, 100, 100));
		renderer->Fill(bounds);
		renderer->SetRenderColor(backColor);
		renderer->FillGradient(position.Left + 1, position.Top + 1, bounds.GetWidth() - 2, bounds.GetHeight() - 2, backColor - Drawing::Color(90, 90, 90));
		renderer->SetRenderColor(backColor - Drawing::Color(0, 50, 50, 50));
		renderer->Fill(position.Left + 5, captionBar.GetBottom() + 2, bounds.GetWidth() - 10, 1);
		//renderer->FillGradient(clientArea, backColor);

		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, captionBar.GetLeft() + 4, captionBar.GetTop() + 2, text);

		for (int i = 0; i < 4; i++)
		{
			renderer->Fill(closeRect.GetLeft() + 4 + i, closeRect.GetTop() + 4 + i, 3, 1);
			renderer->Fill(closeRect.GetLeft() + 10 - i, closeRect.GetTop() + 4 + i, 3, 1);
			renderer->Fill(closeRect.GetLeft() + 4 + i, closeRect.GetTop() + 11 - i, 3, 1);
			renderer->Fill(closeRect.GetLeft() + 10 - i, closeRect.GetTop() + 11 - i, 3, 1);
		}
		
		Drawing::Rectangle rect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea);
	
		for (unsigned int i = 0, len = Controls.size(); i < len; i++)
		{
			Controls.at(i)->Render(renderer);
		}
		
		renderer->SetRenderRectangle(rect);
	}
	//---------------------------------------------------------------------------
}