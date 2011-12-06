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
	TabControl::TabControl()
		: Control()
	{
		type = CONTROL_TABCONTROL;

		SetSize(DefaultSize);
		
		SetBackColor(Drawing::Color(0xFF737373));
		SetForeColor(Drawing::Color(0xFFE5E0E4));

		canRaiseEvents = false;
	}
	//---------------------------------------------------------------------------
	TabControl::~TabControl()
	{

	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TabControl::SetSize(const Drawing::Size &size)
	{
		Control::SetSize(size);

		//anzahl begrenzen
	}
	//---------------------------------------------------------------------------
	void TabControl::SetForeColor(const Drawing::Color &color)
	{
		Control::SetForeColor(color);

		for (std::list<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			(*it)->button->SetForeColor(color);
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::SetBackColor(const Drawing::Color &color)
	{
		Control::SetBackColor(color);

		for (std::list<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			(*it)->button->SetBackColor(color);
		}
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetTabPage(const Misc::AnsiString &text) const
	{
		for (std::list<TabPageButtonBinding*>::const_iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it)->tabPage->GetText() == text)
			{
				return (*it)->tabPage;
			}
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetTabPage(int index) const
	{
		if (index > 0 && index < (int)bindings.size())
		{
			std::list<TabPageButtonBinding*>::const_iterator it = bindings.begin();
			for (int i = 0; i < index; ++i, ++it);
			return (*it)->tabPage;
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	void TabControl::SetSelectedIndex(int index)
	{
		for (std::list<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it)->index == index)
			{
				selected->button->SetActive(false);
				selected = *it;
				selected->button->SetActive(true);

				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	int TabControl::GetSelectedIndex() const
	{
		return selected->index;
	}
	//---------------------------------------------------------------------------
	void TabControl::SetSelectedTabPage(TabPage *tabPage)
	{
		for (std::list<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it)->tabPage == tabPage)
			{
				selected->button->SetActive(false);
				selected = *it;
				selected->button->SetActive(true);

				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetSelectedTabPage() const
	{
		return selected->tabPage;
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

		for (std::list<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it)->tabPage == tabPage)
			{
				return;
			}
		}

		tabPage->SetSize(size);

		TabPageButtonBinding *binding = new TabPageButtonBinding();
		binding->index = bindings.size();
		binding->tabPage = tabPage;

		TabControlButton *button = new TabControlButton(binding);
		button->SetForeColor(GetForeColor());
		button->SetBackColor(GetBackColor());

		tabPage->button = button;
		binding->button = button;

		if (bindings.empty())
		{
			button->SetActive(true);
			selected = binding;
		}
		bindings.push_back(binding);
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

		for (std::list<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it)->tabPage == tabPage)
			{
				TabPageButtonBinding *binding = *it;
				TabPage *temp = binding->tabPage;
				delete binding->button;
				binding->tabPage->button = 0;
				bindings.erase(it);

				if (selected->tabPage == temp)
				{
					if (!bindings.empty())
					{
						selected = bindings.front();
						selected->button->SetActive(true);
					}
					else
					{
						selected->index = -1;
						selected->tabPage = 0;
						selected->button = 0;
					}
				}

				break;
			}
		}
	}
	//---------------------------------------------------------------------------
	bool TabControl::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void TabControl::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();


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

		for (std::list<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			(*it)->button->Render(renderer);
		}
		if (selected->tabPage != 0)
		{
			selected->tabPage->Render(renderer);
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
	}
	//---------------------------------------------------------------------------
	const Drawing::Point TabControl::TabControlButton::DefaultLabelOffset(4, 7);
	//---------------------------------------------------------------------------
	TabControl::TabControlButton::TabControlButton(TabPageButtonBinding *binding)
	{
		this->binding = binding;

		label = new Label();
		label->SetLocation(DefaultLabelOffset);
		label->SetText(binding->tabPage->GetText());

		size = label->GetSize().InflateEx(DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2);
	}
	//---------------------------------------------------------------------------
	TabControl::TabControlButton::~TabControlButton()
	{
		delete label;
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::SetForeColor(const Drawing::Color &color)
	{
		Control::SetForeColor(color);

		label->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::SetBackColor(const Drawing::Color &color)
	{
		Control::SetBackColor(color);

		label->SetBackColor(color);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::SetText(const Misc::AnsiString &text)
	{
		label->SetText(text);

		size = label->GetSize().InflateEx(DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::SetActive(bool active)
	{
		this->active = active;
	}
	//---------------------------------------------------------------------------
	bool TabControl::TabControlButton::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::CalculateAbsoluteLocation()
	{
		Control::CalculateAbsoluteLocation();

		label->SetParent(this);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::OnMouseClick(const MouseMessage &mouse)
	{
		Control::OnMouseClick(mouse);

		if (!active)
		{
			if (parent != 0)
			{
				TabControl *tc = static_cast<TabControl*>(parent);
				tc->SetSelectedIndex(binding->index);
			}
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::Render(Drawing::IRenderer *renderer)
	{
		if (active)
		{
			renderer->SetRenderColor(backColor + Drawing::Color(0, 43, 43, 43));
			renderer->FillGradient(absoluteLocation.Left, absoluteLocation.Top, size.Width, size.Height, backColor - Drawing::Color(0, 10, 10, 10));
			renderer->SetRenderColor(backColor);
			renderer->FillGradient(absoluteLocation.Left + 1, absoluteLocation.Top + 1, size.Width - 2, size.Height, backColor - Drawing::Color(0, 42, 42, 42));
		}
		else
		{
			Drawing::Color backInactive = backColor - Drawing::Color(0, 47, 47, 47);
			Drawing::Color backInactiveGradient = backInactive - Drawing::Color(0, 20, 20, 20);
			Drawing::Color borderInactive = backInactive + Drawing::Color(0, 9, 9, 9);

			renderer->SetRenderColor(borderInactive);
			renderer->Fill(absoluteLocation.Left, absoluteLocation.Top, size.Width, size.Height - 1);
			renderer->SetRenderColor(backInactive);
			renderer->FillGradient(absoluteLocation.Left + 1, absoluteLocation.Top + 1, size.Width - 2, size.Height - 1, backInactiveGradient);
		}

		label->Render(renderer);
	}
	//---------------------------------------------------------------------------
}