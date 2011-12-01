#include "TabControl.hpp"
#include "TabPage.hpp"
#include "..\Misc\TextHelper.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TabControl::TabControl() : Control()
	{
		type = CONTROL_TABCONTROL;

		activeTab = 0;
		
		SetBounds(6, 6, 200, 100);
		
		SetBackColor(Drawing::Color(0xFF737373));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	TabControl::~TabControl()
	{
		for (std::list<TabPage*>::iterator it = tabs.begin(); it != tabs.end(); ++it)
		{
			delete *it;
		}

		tabs.clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetTabPage(const Misc::AnsiString &text) const
	{
		for (std::list<TabPage*>::const_iterator it = tabs.begin(); it != tabs.end(); ++it)
		{
			if ((*it)->GetText() == text)
			{
				return *it;
			}
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetTabPage(int index) const
	{
		if (index > 0 && index < (int)tabs.size())
		{
			std::list<TabPage*>::const_iterator it = tabs.begin();
			for (int i = 0; i < index; ++i)
			{
				++it;
			}

			return *it;
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	SelectedIndexChangedEvent& TabControl::GetSelectedIndexChangedEvent()
	{
		return selectedIndexChangedEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void TabControl::AddTabPage(TabPage *tabPage)
	{
		if (tabPage == 0)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException("tabPage", __FILE__, __LINE__);
			#endif
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
	void TabControl::RemoveTabPage(TabPage *tabPage)
	{
		if (tabPage == 0)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException("tabPage", __FILE__, __LINE__);
			#endif
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
	bool TabControl::Intersect(const Drawing::Point &point) const
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
	bool TabControl::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException("event", __FILE__, __LINE__);
		}

		if (!isVisible || !isEnabled)
		{
			return false;
		}

		Drawing::Point mousePositionBackup;

		if (event->Type == IEvent::Mouse)
		{
			MouseMessage *mouse = (MouseMessage*)event;
			mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			int x = 2;
			Misc::TextHelper textHelper(font);
			if (Drawing::Rectangle(0, 0, bounds.GetWidth(), textHelper.GetSize().Height + 8).Contains(mouse->Position))
			{
				for (std::list<TabPage*>::iterator it = tabs.begin(); it != tabs.end(); ++it)
				{
					textHelper.SetText((*it)->GetText());
					Drawing::Size textSize = textHelper.GetSize();
					if (Drawing::Rectangle(x, 0, textSize.Width + 8, textSize.Height + 8).Contains(mouse->Position))
					{
						static TabPage *clicked = 0;
						if (mouse->State == MouseMessage::LeftDown)
						{
							clicked = *it;

							return true;
						}
						else if (mouse->State == MouseMessage::LeftUp)
						{
							if (clicked == *it)
							{
								parent->RequestFocus(this);
								activeTab = clicked;
								clicked = 0;
								Invalidate();

								selectedIndexChangedEvent.Invoke(this);
							}

							return true;
						}
					}
					x += textSize.Width + 11;
				}
			}

			mouse->Position.Top -= font->GetSize() + 10;
			mouse->Position.Left -= 2;
		}
		else if (event->Type == IEvent::Keyboard)
		{
			KeyboardMessage *keyboard = (KeyboardMessage*)event;

			if (keyboard->State == KeyboardMessage::Character)
			{
				if (keyboard->KeyCode == Key::Left)
				{
					if (activeTab != *tabs.begin())
					{
						for (std::list<TabPage*>::iterator it = tabs.begin(); it != tabs.end(); ++it)
						{
							if (*it == activeTab)
							{
								--it;
								activeTab = *it;

								selectedIndexChangedEvent.Invoke(this);

								break;
							}
						}

						return true;
					}
				}
				else if (keyboard->KeyCode == Key::Right)
				{
					if (activeTab != *tabs.end())
					{
						for (std::list<TabPage*>::iterator it = tabs.begin(); it != tabs.end(); ++it)
						{
							if (*it == activeTab)
							{
								++it;
								activeTab = *it;

								selectedIndexChangedEvent.Invoke(this);

								break;
							}
						}

						return true;
					}
				}
			}
		}
	
		if (ChildProcessEvent(event) == true)
		{
			return true;
		}

		if (activeTab != 0)
		{
			activeTab->ProcessEvent(event);
		}

		return false;
	}
	//---------------------------------------------------------------------------
	void TabControl::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
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

		for (std::list<TabPage*>::iterator it = tabs.begin(); it != tabs.end(); ++it)
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
				renderer->Fill(x, y, textSize.Width + 8, textSize.Height + 7);
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
			
			ChildRender(renderer);
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}