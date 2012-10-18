/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "TabControl.hpp"
#include "TabPage.hpp"
#include "Label.hpp"
#include "../Misc/TextHelper.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::Size TabControl::DefaultSize(200, 200);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TabControl::TabControl()
		: startIndex(0),
		  maxIndex(0)
	{
		type = CONTROL_TABCONTROL;

		lastSwitchButton = new TabControlSwitchButton(0);
		lastSwitchButton->GetClickEvent() += ClickEventHandler([this](Control *control)
		{
			if (startIndex > 0)
			{
				--startIndex;
				CalculateButtonLocationAndCount();
			}
		});
		AddSubControl(lastSwitchButton);

		nextSwitchButton = new TabControlSwitchButton(1);
		nextSwitchButton->GetClickEvent() += ClickEventHandler([this](Control *control)
		{
			if (maxIndex < (int)bindings.size())
			{
				++startIndex;
				CalculateButtonLocationAndCount();
			}
		});
		AddSubControl(nextSwitchButton);

		SetSize(DefaultSize);
		
		ApplyTheme(Application::Instance()->GetTheme());

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

		CalculateButtonLocationAndCount();

		lastSwitchButton->SetLocation(GetWidth() - TabControlSwitchButton::DefaultSize.Width, 0);
		nextSwitchButton->SetLocation(GetWidth() - TabControlSwitchButton::DefaultSize.Width, TabControlSwitchButton::DefaultSize.Height + 1);
	}
	//---------------------------------------------------------------------------
	void TabControl::SetForeColor(const Drawing::Color &color)
	{
		Control::SetForeColor(color);

		for (std::vector<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			TabPageButtonBinding *binding = *it;
			binding->button->SetForeColor(color);
			binding->tabPage->SetForeColor(color);
		}
		lastSwitchButton->SetForeColor(color);
		nextSwitchButton->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	void TabControl::SetBackColor(const Drawing::Color &color)
	{
		Control::SetBackColor(color);

		for (std::vector<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			TabPageButtonBinding *binding = *it;
			binding->button->SetBackColor(color);
			binding->tabPage->SetBackColor(color);
		}
		lastSwitchButton->SetBackColor(color);
		nextSwitchButton->SetBackColor(color);
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetTabPage(const Misc::AnsiString &text) const
	{
		for (std::vector<TabPageButtonBinding*>::const_iterator it = bindings.begin(); it != bindings.end(); ++it)
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
			return bindings[index]->tabPage;
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	void TabControl::SetSelectedIndex(int index)
	{
		for (std::vector<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it)->index == index)
			{
				selected->button->SetActive(false);
				selected->tabPage->SetVisible(false);
				selected = *it;
				selected->button->SetActive(true);
				selected->tabPage->SetVisible(true);
				CalculateButtonLocationAndCount();

				selectedIndexChangedEvent.Invoke(this);

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
		for (std::vector<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it)->tabPage == tabPage)
			{
				selected->tabPage->SetVisible(false);
				selected->button->SetActive(false);
				selected = *it;
				selected->button->SetActive(true);
				selected->tabPage->SetVisible(true);
				CalculateButtonLocationAndCount();

				selectedIndexChangedEvent.Invoke(this);

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
		if (tabPage == nullptr)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException("tabPage", __FILE__, __LINE__);
			#endif
			return;
		}

		for (std::vector<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
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
		button->SetLocation(Drawing::Point(0, 0));
		button->SetForeColor(GetForeColor());
		button->SetBackColor(GetBackColor());
		button->SetFont(font);

		AddSubControl(button);
		AddSubControl(tabPage);

		tabPage->button = button;
		binding->button = button;

		if (bindings.empty())
		{
			button->SetActive(true);
			tabPage->SetVisible(true);
			selected = binding;
			tabPage->SetLocation(0, button->GetSize().Height);
		}
		else
		{
			tabPage->SetVisible(false);
		}
		bindings.push_back(binding);

		CalculateButtonLocationAndCount();
	}
	//---------------------------------------------------------------------------
	void TabControl::RemoveTabPage(TabPage *tabPage)
	{
		if (tabPage == nullptr)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException("tabPage", __FILE__, __LINE__);
			#endif
			return;
		}

		for (std::vector<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it)->tabPage == tabPage)
			{
				TabPageButtonBinding *binding = *it;
				TabPage *temp = binding->tabPage;

				RemoveControl(binding->button);
				RemoveControl(binding->tabPage);

				delete binding->button;
				binding->tabPage->button = nullptr;
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
						selected->tabPage = nullptr;
						selected->button = nullptr;
					}
				}

				break;
			}
		}

		CalculateButtonLocationAndCount();
	}
	//---------------------------------------------------------------------------
	bool TabControl::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void TabControl::CalculateAbsoluteLocation()
	{
		ContainerControl::CalculateAbsoluteLocation();

		CalculateButtonLocationAndCount();
	}
	//---------------------------------------------------------------------------
	void TabControl::CalculateButtonLocationAndCount()
	{
		if (!bindings.empty())
		{
			maxIndex = startIndex;

			for (unsigned int i = 0; i < bindings.size(); ++i)
			{
				bindings[i]->button->SetVisible(false);
			}

			int tempWidth = 0;
			int maxWidth = size.Width - TabControlSwitchButton::DefaultSize.Width;
			for (int i = startIndex; i < (int)bindings.size(); ++i)
			{
				TabControlButton *button = bindings[i]->button;
				if (tempWidth + button->GetSize().Width <= maxWidth)
				{
					button->SetLocation(tempWidth, 0);
					button->SetVisible(true);

					++maxIndex;
					tempWidth += button->GetSize().Width + 2;
				}
				else
				{
					break;
				}
			}

			selected->tabPage->SetLocation(0, selected->button->GetSize().Height);

			if (startIndex != 0)
			{
				lastSwitchButton->SetVisible(true);
			}
			else
			{
				lastSwitchButton->SetVisible(false);
			}
			if (maxIndex < (int)bindings.size())
			{
				nextSwitchButton->SetVisible(true);
			}
			else
			{
				nextSwitchButton->SetVisible(false);
			}
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		if (selected->tabPage != nullptr)
		{
			for (int i = startIndex; i < maxIndex; ++i)
			{
				bindings[i]->button->Render(renderer);
			}
		
			nextSwitchButton->Render(renderer);
			lastSwitchButton->Render(renderer);

			selected->tabPage->Render(renderer);
		}
	}
	//---------------------------------------------------------------------------
	const Drawing::Point TabControl::TabControlButton::DefaultLabelOffset(4, 2);
	//---------------------------------------------------------------------------
	TabControl::TabControlButton::TabControlButton(TabPageButtonBinding *binding)
	{
		this->binding = binding;

		active = false;

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
			if (parent != nullptr)
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
			Drawing::Color base = backColor;
			if (isInside)
			{
				base += Drawing::Color(0, 50, 50, 50);
			}

			Drawing::Color backInactive = base - Drawing::Color(0, 47, 47, 47);
			Drawing::Color backInactiveGradient = backInactive - Drawing::Color(0, 20, 20, 20);
			Drawing::Color borderInactive = backInactive + Drawing::Color(0, 9, 9, 9);

			renderer->SetRenderColor(borderInactive);
			renderer->Fill(absoluteLocation.Left, absoluteLocation.Top, size.Width, size.Height);
			renderer->SetRenderColor(backInactive);
			renderer->FillGradient(absoluteLocation.Left + 1, absoluteLocation.Top + 1, size.Width - 2, size.Height - 1, backInactiveGradient);
		}

		label->Render(renderer);
	}
	//---------------------------------------------------------------------------
	const Drawing::Size TabControl::TabControlSwitchButton::DefaultSize(9, 9);
	//---------------------------------------------------------------------------
	TabControl::TabControlSwitchButton::TabControlSwitchButton(int direction)
	{
		this->direction = direction;

		SetSize(DefaultSize);
	}
	//---------------------------------------------------------------------------
	bool TabControl::TabControlSwitchButton::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlSwitchButton::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		Drawing::Color base = isInside ? backColor : backColor - Drawing::Color(0, 47, 47, 47);
		Drawing::Color borderColor = backColor + Drawing::Color(0, 9, 9, 9);
		renderer->SetRenderColor(borderColor);
		renderer->Fill(absoluteLocation, size);
		renderer->SetRenderColor(base);
		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top + 1, size.Width - 2, size.Height - 2);
		
		int x = absoluteLocation.Left + 3;
		renderer->SetRenderColor(foreColor);
		if (direction == 0)
		{
			int y = absoluteLocation.Top + 4;
			for (int i = 0; i < 3; ++i)
			{
				renderer->Fill(x + i, y - i, 1, 1 + i * 2);
			}
		}
		else
		{
			int y = absoluteLocation.Top + 2;
			for (int i = 0; i < 3; ++i)
			{
				renderer->Fill(x + i, y + i, 1, 5 - i * 2);
			}
		}
	}
	//---------------------------------------------------------------------------
}