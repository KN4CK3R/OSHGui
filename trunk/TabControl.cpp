#include "TabControl.h"
#include "TabPage.h"
#include "Misc\TextHelper.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TabControl::TabControl(const std::shared_ptr<Control> &parent) : Control(parent)
	{
		type = CONTROL_TABCONTROL;

		activeTab = 0;
		
		SetBackColor(Drawing::Color(0xFF737373));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	std::shared_ptr<TabPage> TabControl::GetTabPage(const Misc::UnicodeString &text)
	{
		for (std::list<std::shared_ptr<TabPage>>::iterator it = tabs.begin(); it != tabs.end(); it++)
		{
			if ((*it)->GetText() == text)
			{
				return *it;
			}
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	std::shared_ptr<TabPage> TabControl::GetTabPage(int index)
	{
		if (index > 0 && index < (int)tabs.size())
		{
			std::list<std::shared_ptr<TabPage>>::iterator it = tabs.begin();
			for (int i = 0; i < index; i++)
			{
				it++;
			}

			return *it;
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	void TabControl::AddTabPage(const std::shared_ptr<TabPage> &tabPage)
	{
		if (tabPage == 0)
		{
			return;
		}

		tabs.push_back(tabPage);

		if (activeTab == 0)
		{
			activeTab = tabPage;
			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::RemoveTabPage(const std::shared_ptr<TabPage> &tabPage)
	{
		if (tabPage == 0)
		{
			return;
		}

		tabs.remove(tabPage);

		if (activeTab == tabPage)
		{
			if (tabs.size() > 0)
			{
				activeTab = *tabs.begin();
				Invalidate();
			}
			else
			{
				activeTab = 0;
			}
		}
	}
	//---------------------------------------------------------------------------
	bool TabControl::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void TabControl::Invalidate()
	{
		clientArea = bounds.InflateEx(-2, -font->GetSize() + 4).OffsetEx(1, font->GetSize() + 8);

		if (activeTab != 0)
		{
			activeTab->SetBounds(clientArea);
		}

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes TabControl::ProcessEvent(const std::shared_ptr<Event> &event)
	{
		if (event == 0)
		{
			return Event::DontContinue;
		}

		if (!visible || !enabled)
		{
			return Event::Continue;
		}

		Drawing::Point mousePositionBackup;

		if (event->Type == Event::Mouse)
		{
			std::shared_ptr<MouseEvent> mouse = std::static_pointer_cast<MouseEvent>(event);
			mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			int x = 2;
			Misc::TextHelper textHelper(font);
			for (std::list<std::shared_ptr<TabPage>>::iterator it = tabs.begin(); it != tabs.end(); it++)
			{
				textHelper.SetText((*it)->GetText());
				Drawing::Size textSize = textHelper.GetSize();
				if (Drawing::Rectangle(x, 0, textSize.Width + 8, textSize.Height + 8).Contains(mouse->Position))
				{
					static std::shared_ptr<TabPage> clicked = 0;
					if (mouse->State == MouseEvent::LeftDown)
					{
						clicked = *it;

						return Event::DontContinue;
					}
					else if (mouse->State == MouseEvent::LeftUp)
					{
						if (clicked == *it)
						{
							Parent->RequestFocus(shared_from_this());
							activeTab = clicked;
							clicked = 0;
							Invalidate();
						}

						return Event::DontContinue;
					}
				}
				x += textSize.Width + 11;
			}

			mouse->Position.Top -= font->GetSize() + 8;
		}
	
		if (ProcessChildrenEvent(event) == Event::DontContinue)
		{
			return Event::DontContinue;
		}

		if (activeTab != 0)
		{
			activeTab->ProcessEvent(event);
		}

		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void TabControl::Render(const std::shared_ptr<Drawing::IRenderer> &renderer)
	{
		if (!visible)
		{
			return;
		}

		if (activeTab != 0)
		{
			activeTab->Render(renderer);
		}
	
		int x = bounds.GetLeft() + 2;
		int y = bounds.GetTop();
		Misc::TextHelper textHelper(font);
		Drawing::Color backInactive = backColor - Drawing::Color(0, 47, 47, 47);
		Drawing::Color backInactiveGradient = backInactive - Drawing::Color(0, 20, 20, 20);
		Drawing::Color borderInactive = backInactive + Drawing::Color(0, 9, 9,9);

		for (std::list<std::shared_ptr<TabPage>>::iterator it = tabs.begin(); it != tabs.end(); it++)
		{
			textHelper.SetText((*it)->GetText());
			Drawing::Size textSize = textHelper.GetSize();

			if (*it == activeTab)
			{
				renderer->SetRenderColor(backColor + Drawing::Color(0, 43, 43, 43));
				renderer->FillGradient(x, y, textSize.Width + 8, textSize.Height + 8, backColor - Drawing::Color(0, 10, 10, 10));
				renderer->SetRenderColor(backColor);
				renderer->FillGradient(x + 1, y + 1, textSize.Width + 6, textSize.Height + 8, backColor - Drawing::Color(0, 42, 42, 42));
			}
			else
			{
				renderer->SetRenderColor(borderInactive);
				renderer->Fill(x, y, textSize.Width + 8, textSize.Height + 8);
				renderer->SetRenderColor(backInactive);
				renderer->FillGradient(x + 1, y + 1, textSize.Width + 6, textSize.Height + 7, backInactiveGradient);
			}
			renderer->SetRenderColor(foreColor);
			renderer->RenderText(font, x + 4, y + 4, textSize.Width, textSize.Height, textHelper.GetText());
			x += textSize.Width + 11;
		}
	
		if (controls.size() > 0)
		{
			Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
			renderer->SetRenderRectangle(clientArea + renderRect.GetPosition());
			
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				controls.at(i)->Render(renderer);
			}
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}