#include "TabControl.hpp"
#include "TabPage.hpp"
#include "Label.hpp"
#include "..\Misc\TextHelper.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::Size TabControl::DefaultSize(200, 100);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TabControl::TabControl() : Control()
	{
		type = CONTROL_TABCONTROL;
		
		tabControlBar = new TabControlBar();
		tabControlBar->SetLocation(Drawing::Point(0, 0));

		SetSize(DefaultSize);
		
		SetBackColor(Drawing::Color(0xFF737373));
		SetForeColor(Drawing::Color(0xFFE5E0E4));

		canRaiseEvents = false;
	}
	//---------------------------------------------------------------------------
	TabControl::~TabControl()
	{
		delete tabControlBar;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TabControl::SetSize(const Drawing::Size &size)
	{
		Control::SetSize(size);

		tabControlBar->SetSize(size);
	}
	//---------------------------------------------------------------------------
	void TabControl::SetForeColor(const Drawing::Color &color)
	{
		Control::SetForeColor(color);

		tabControlBar->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	void TabControl::SetBackColor(const Drawing::Color &color)
	{
		Control::SetBackColor(color);

		tabControlBar->SetBackColor(color);
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetTabPage(const Misc::AnsiString &text) const
	{
		const std::list<TabControlBar::TabPageButtonBinding> &bindings = tabControlBar->GetTabPageButtonBindings();
		for (std::list<TabControlBar::TabPageButtonBinding>::const_iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it).tabPage->GetText() == text)
			{
				return (*it).tabPage;
			}
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetTabPage(int index) const
	{
		const std::list<TabControlBar::TabPageButtonBinding> &bindings = tabControlBar->GetTabPageButtonBindings();
		if (index > 0 && index < (int)bindings.size())
		{
			std::list<TabControlBar::TabPageButtonBinding>::const_iterator it = bindings.begin();
			for (int i = 0; i < index; ++i, ++it);
			return (*it).tabPage;
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	void TabControl::SetSelectedIndex(int index)
	{

	}
	//---------------------------------------------------------------------------
	int TabControl::GetSelectedIndex() const
	{

	}
	//---------------------------------------------------------------------------
	void TabControl::SetSelectedTabPage(TabPage *tabPage)
	{
		tabControlBar->SetSelectedTabPage(tabPage);
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetSelectedTabPage() const
	{
		tabControlBar->GetSelectedTabPage();
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

		tabControlBar->AddTabPage(tabPage);
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

		tabControlBar->RemoveTabPage(tabPage);
	}
	//---------------------------------------------------------------------------
	bool TabControl::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	/*//---------------------------------------------------------------------------
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
	//---------------------------------------------------------------------------*/
	void TabControl::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		/*if (activeTab != 0)
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
		}*/

		tabControlBar->Render(renderer);
	}
	//---------------------------------------------------------------------------
	TabControl::TabControlBar::TabControlBar()
	{

	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlBar::SetForeColor(const Drawing::Color &color)
	{
		ContainerControl::SetForeColor(color);

		const std::list<Control*> &controls = GetControls();
		for (std::list<Control*>::const_iterator it = controls.begin(); it != controls.end(); ++it)
		{
			(*it)->SetForeColor(color);
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlBar::SetBackColor(const Drawing::Color &color)
	{
		ContainerControl::SetBackColor(color);

		const std::list<Control*> &controls = GetControls();
		for (std::list<Control*>::const_iterator it = controls.begin(); it != controls.end(); ++it)
		{
			(*it)->SetBackColor(color);
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlBar::SetSelectedTabPage(TabPage *tabPage)
	{
		bool found = false;
		for (std::list<TabPageButtonBinding>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it).tabPage == tabPage)
			{
				found = true;
				break;
			}
		}

		if (found)
		{
			selectedTabPage = tabPage;
		}
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::TabControlBar::GetSelectedTabPage() const
	{
		return selectedTabPage;
	}
	//---------------------------------------------------------------------------
	const std::list<TabPageButtonBinding>& TabControl::TabControlBar::GetTabPageButtonBindings() const
	{
		return bindings;
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlBar::AddTabPage(TabPage *tabPage)
	{
		if (tabPage == 0)
		{
			return;
		}

		bool found = false;
		for (std::list<TabPageButtonBinding>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it).tabPage == tabPage)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			TabControlBarButton *button = new TabControlBarButton(tabPage);
			button->SetForeColor(GetForeColor());
			button->SetBackColor(GetBackColor());
			TabPageButtonBinding binding = { bindings.size(), tabPage, button };
			tabPage->button = button;

			AddControl(button);

			bindings.push_back(binding);
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlBar::RemoveTabPage(TabPage *tabPage)
	{
		for (std::list<TabPageButtonBinding>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it).tabPage == tabPage)
			{
				TabPageButtonBinding &binding = *it;

				RemoveControl(binding.button);
				delete binding.button;
				binding.tabPage->button = 0;
				bindings.erase(it);

				break;
			}
		}

		if (selectedTabPage == tabPage)
		{
			if (!bindings.empty())
			{
				selectedTabPage = bindings.front().tabPage;
			}
			else
			{
				selectedTabPage = 0;
			}
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlBar::Render(Drawing::IRenderer *renderer)
	{

	}
	//---------------------------------------------------------------------------
	const Drawing::Point TabControl::TabControlBar::TabControlBarButton::DefaultLabelOffset(4, 7);
	//---------------------------------------------------------------------------
	TabControl::TabControlBar::TabControlBarButton::TabControlBarButton(TabPage *tabPage)
	{
		this->tabPage = tabPage;

		label = new Label();
		label->SetLocation(DefaultLabelOffset);
		label->SetText(tabPage->GetText());

		size = label->GetSize().InflateEx(DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2);
	}
	//---------------------------------------------------------------------------
	TabControl::TabControlBar::TabControlBarButton::~TabControlBarButton()
	{
		delete label;
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlBar::TabControlBarButton::SetForeColor(const Drawing::Color &color)
	{
		Control::SetForeColor(color);

		label->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlBar::TabControlBarButton::SetBackColor(const Drawing::Color &color)
	{
		Control::SetBackColor(color);

		label->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlBar::TabControlBarButton::SetText(const Misc::AnsiString &text)
	{
		label->SetText(text);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlBar::TabControlBarButton::SetActive(bool active)
	{
		this->active = active;
	}
	//---------------------------------------------------------------------------
	bool TabControl::TabControlBar::TabControlBarButton::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlBar::TabControlBarButton::Render(Drawing::IRenderer *renderer)
	{
		size = label->GetSize().InflateEx(DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2);

		Drawing::Color backInactive = backColor - Drawing::Color(0, 47, 47, 47);
		Drawing::Color backInactiveGradient = backInactive - Drawing::Color(0, 20, 20, 20);
		Drawing::Color borderInactive = backInactive + Drawing::Color(0, 9, 9, 9);

		if (active)
		{
			renderer->SetRenderColor(backColor + Drawing::Color(0, 43, 43, 43));
			renderer->FillGradient(absoluteLocation.Left, absoluteLocation.Top, size.Width, size.Height, backColor - Drawing::Color(0, 10, 10, 10));
			renderer->SetRenderColor(backColor);
			renderer->FillGradient(absoluteLocation.Left + 1, absoluteLocation.Top + 1, size.Width - 2, size.Height, backColor - Drawing::Color(0, 42, 42, 42));
		}
		else
		{
			renderer->SetRenderColor(borderInactive);
			renderer->Fill(absoluteLocation.Left, absoluteLocation.Top, size.Width, size.Height - 1);
			renderer->SetRenderColor(backInactive);
			renderer->FillGradient(absoluteLocation.Left + 1, absoluteLocation.Top + 1, size.Width - 2, size.Height - 1, backInactiveGradient);
		}

		label->Render(renderer);
	}
	//---------------------------------------------------------------------------
}