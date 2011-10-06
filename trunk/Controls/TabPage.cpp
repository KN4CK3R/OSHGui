#include "TabPage.h"
#include "TabControl.h"
#include "..\Misc\Exceptions.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TabPage::TabPage(TabControl *parent) : Panel(parent)
	{
		type = CONTROL_TABPAGE;
		
		text = L"TabPage";
		
		SetBackColor(Drawing::Color(0xFF474747));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TabPage::SetText(const Misc::UnicodeString &text)
	{
		this->text = text;
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& TabPage::GetText() const
	{
		return text;
	}
	//---------------------------------------------------------------------------
	void TabPage::Invalidate()
	{
		clientArea = bounds.InflateEx(-4, -4).OffsetEx(2, 2);

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes TabPage::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}
	
		if (ChildProcessEvent(event) == IEvent::DontContinue)
		{
			return IEvent::DontContinue;
		}

		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;

			if (Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight()).Contains(mouse->Position))
			{
				if (mouse->State == MouseEvent::LeftDown || mouse->State == MouseEvent::RightDown)
				{
					mouseDownEvent.Invoke(this, MouseEventArgs(mouse));
				}
				else if (mouse->State == MouseEvent::Move)
				{
					mouseMoveEvent.Invoke(this, MouseEventArgs(mouse));
				}
				else if (mouse->State == MouseEvent::LeftUp || mouse->State == MouseEvent::RightUp)
				{
					mouseUpEvent.Invoke(this, MouseEventArgs(mouse));
				}
				
				return IEvent::DontContinue;
			}
		}

		return IEvent::Continue;
	}
	//---------------------------------------------------------------------------
	void TabPage::Render(Drawing::IRenderer *renderer)
	{
		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor + Drawing::Color(0, 32, 32, 32));
			renderer->Fill(bounds);
			renderer->SetRenderColor(backColor);
			renderer->FillGradient(bounds.GetLeft() + 1, bounds.GetTop() + 1, bounds.GetWidth() - 2, bounds.GetHeight() - 2, backColor - Drawing::Color(0, 20, 20, 20));
		}
	
		if (controls.size() > 0)
		{
			Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
			renderer->SetRenderRectangle(clientArea.InflateEx(-4, -4).OffsetEx(2, 2) + renderRect.GetPosition());
			
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				controls[i]->Render(renderer);
			}
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}