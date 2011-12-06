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
		: ContainerControl()
	{
		type = CONTROL_TABCONTROL;

		SetSize(DefaultSize);

		maxVisibleButtons = 0;
		
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

		CalculateButtonLocationAndCount();
	}
	//---------------------------------------------------------------------------
	void TabControl::SetForeColor(const Drawing::Color &color)
	{
		Control::SetForeColor(color);

		for (std::vector<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			(*it)->button->SetForeColor(color);
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::SetBackColor(const Drawing::Color &color)
	{
		Control::SetBackColor(color);

		for (std::vector<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			(*it)->button->SetBackColor(color);
		}
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
			std::vector<TabPageButtonBinding*>::const_iterator it = bindings.begin();
			for (int i = 0; i < index; ++i, ++it);
			return (*it)->tabPage;
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
				selected = *it;
				selected->button->SetActive(true);
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
				selected->button->SetActive(false);
				selected = *it;
				selected->button->SetActive(true);
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
		if (tabPage == 0)
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
			selected = binding;
		}
		bindings.push_back(binding);

		CalculateButtonLocationAndCount();
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

		for (std::vector<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			if ((*it)->tabPage == tabPage)
			{
				TabPageButtonBinding *binding = *it;
				TabPage *temp = binding->tabPage;

				RemoveControl(binding->button);
				RemoveControl(binding->tabPage);

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
		Control::CalculateAbsoluteLocation();

		CalculateButtonLocationAndCount();
	}
	//---------------------------------------------------------------------------
	void TabControl::CalculateButtonLocationAndCount()
	{
		if (!bindings.empty())
		{
			maxVisibleButtons = 0;

			int tempWidth = 0;
			for (std::vector<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
			{
				TabControlButton *button = (*it)->button;
				if (tempWidth + button->GetSize().Width <= size.Width)
				{
					button->SetLocation(tempWidth, 0);

					++maxVisibleButtons;
					tempWidth += button->GetSize().Width + 2;
				}
				else
				{
					break;
				}
			}

			selected->tabPage->SetLocation(0, selected->button->GetSize().Height);
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}

		for (std::vector<TabPageButtonBinding*>::iterator it = bindings.begin(); it != bindings.end(); ++it)
		{
			(*it)->button->Render(renderer);
		}
		if (selected->tabPage != 0)
		{
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
			Drawing::Color base = backColor;
			if (isInside)
			{
				base += Drawing::Color(0, 50, 50, 50);
			}

			Drawing::Color backInactive = base - Drawing::Color(0, 47, 47, 47);
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