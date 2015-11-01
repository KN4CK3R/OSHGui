/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "TabControl.hpp"
#include "TabPage.hpp"
#include "Label.hpp"
#include "../Misc/TextHelper.hpp"
#include "../Misc/Exceptions.hpp"
#include <algorithm>

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeI TabControl::DefaultSize(200, 200);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TabControl::TabControl()
		: startIndex(0),
		  maxIndex(0),
		  selected(nullptr)
	{
		type_ = ControlType::TabControl;

		lastSwitchButton = new TabControlSwitchButton(TabControlSwitchButton::TabControlSwitchButtonDirection::Left);
		lastSwitchButton->GetClickEvent() += ClickEventHandler([this](Control *control)
		{
			if (startIndex > 0)
			{
				--startIndex;
				CalculateButtonLocationAndCount();
			}
		});
		AddSubControl(lastSwitchButton);

		nextSwitchButton = new TabControlSwitchButton(TabControlSwitchButton::TabControlSwitchButtonDirection::Right);
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
		
		ApplyStyle(Application::Instance().GetStyle());

		canRaiseEvents_ = false;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TabControl::SetSize(const Drawing::SizeI &size)
	{
		Control::SetSize(size);

		CalculateButtonLocationAndCount();

		lastSwitchButton->SetLocation(GetWidth() - TabControlSwitchButton::DefaultSize.Width, 2);
		nextSwitchButton->SetLocation(GetWidth() - TabControlSwitchButton::DefaultSize.Width, 2 + TabControlSwitchButton::DefaultSize.Height + 1);

		if (selected != nullptr)
		{
			auto tabPageSize = size.InflateEx(0, -selected->Button->GetHeight() - ButtonPageDistance);

			for (auto &binding : bindings)
			{
				binding->TabPage->SetSize(tabPageSize);
			}
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::SetForeColor(const Drawing::Color &color)
	{
		Control::SetForeColor(color);

		lastSwitchButton->SetForeColor(color);
		nextSwitchButton->SetForeColor(color);

		for (auto &binding : bindings)
		{
			binding->Button->SetForeColor(color);
			binding->TabPage->SetForeColor(color);
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::SetBackColor(const Drawing::Color &color)
	{
		Control::SetBackColor(color);

		lastSwitchButton->SetBackColor(color);
		nextSwitchButton->SetBackColor(color);

		for (auto &binding : bindings)
		{
			binding->Button->SetBackColor(color);
			binding->TabPage->SetBackColor(color);
		}
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetTabPage(const Misc::AnsiString &text) const
	{
		for (auto &binding : bindings)
		{
			if (binding->TabPage->GetText() == text)
			{
				return binding->TabPage;
			}
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetTabPage(int index) const
	{
		if (index > 0 && index < (int)bindings.size())
		{
			return bindings[index]->TabPage;
		}

		return 0;
	}
	//---------------------------------------------------------------------------
	void TabControl::SetSelectedIndex(int index)
	{
		for (auto &binding : bindings)
		{
			if (binding->Index == index)
			{
				SelectBinding(*binding);

				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	int TabControl::GetSelectedIndex() const
	{
		return selected->Index;
	}
	//---------------------------------------------------------------------------
	void TabControl::SetSelectedTabPage(TabPage *tabPage)
	{
		for (auto &binding : bindings)
		{
			if (binding->TabPage == tabPage)
			{
				SelectBinding(*binding);

				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetSelectedTabPage() const
	{
		return selected->TabPage;
	}
	//---------------------------------------------------------------------------
	SelectedIndexChangedEvent& TabControl::GetSelectedIndexChangedEvent()
	{
		return selectedIndexChangedEvent_;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void TabControl::AddTabPage(TabPage *tabPage)
	{
		if (tabPage == nullptr)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException("tabPage");
			#endif
			return;
		}

		for (auto &binding : bindings)
		{
			if (binding->TabPage == tabPage)
			{
				return;
			}
		}

		std::unique_ptr<TabPageButtonBinding> binding(new TabPageButtonBinding());
		binding->Index = bindings.size();
		binding->TabPage = tabPage;

		auto button = new TabControlButton(*binding);
		button->SetLocation(Drawing::PointI(0, 0));
		button->SetForeColor(GetBackColor());
		button->SetBackColor(GetForeColor());

		AddSubControl(button);

		tabPage->SetSize(size_.InflateEx(0, -button->GetHeight()));

		AddSubControl(tabPage);

		tabPage->button = button;
		binding->Button = button;

		if (bindings.empty())
		{
			button->SetActive(true);
			tabPage->SetVisible(true);
			selected = binding.get();
			tabPage->SetLocation(0, button->GetSize().Height + ButtonPageDistance);
		}
		else
		{
			tabPage->SetVisible(false);
		}

		bindings.push_back(std::move(binding));

		CalculateButtonLocationAndCount();
	}
	//---------------------------------------------------------------------------
	void TabControl::RemoveTabPage(TabPage *tabPage)
	{
		if (tabPage == nullptr)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException("tabPage");
			#endif
			return;
		}

		for (auto &binding : bindings)
		{
			if (binding->TabPage == tabPage)
			{
				RemoveControl(binding->Button);
				delete binding->Button;
				binding->TabPage->button = nullptr;
				RemoveControl(binding->TabPage);

				bindings.erase(std::remove(std::begin(bindings), std::end(bindings), binding), std::end(bindings));

				if (selected->TabPage == tabPage)
				{
					if (!bindings.empty())
					{
						selected = bindings.front().get();
						selected->Button->SetActive(true);
					}
					else
					{
						selected->Index = -1;
						selected->TabPage = nullptr;
						selected->Button = nullptr;
					}
				}

				break;
			}
		}

		CalculateButtonLocationAndCount();
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
			maxIndex = startIndex;

			int tempWidth = 0;
			int maxWidth = size_.Width - TabControlSwitchButton::DefaultSize.Width;
			for (auto &binding : bindings)
			{
				auto button = binding->Button;
				button->SetVisible(false);

				if (tempWidth + button->GetSize().Width <= maxWidth)
				{
					button->SetLocation(tempWidth, 0);
					button->SetVisible(true);

					++maxIndex;
					tempWidth += button->GetSize().Width + 3;
				}
				else
				{
					break;
				}
			}

			if (selected != nullptr)
			{
				selected->TabPage->SetLocation(0, selected->Button->GetSize().Height + ButtonPageDistance);
			}

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

			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::SelectBinding(TabPageButtonBinding &binding)
	{
		selected->TabPage->SetVisible(false);
		selected->Button->SetActive(false);
		selected = &binding;
		selected->Button->SetActive(true);
		selected->TabPage->SetVisible(true);
		CalculateButtonLocationAndCount();

		selectedIndexChangedEvent_.Invoke(this);

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void TabControl::ApplyStyle(const Drawing::Style &style)
	{
		Control::ApplyStyle(style);
		
		for (auto &binding : bindings)
		{
			binding->TabPage->ApplyStyle(style);
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::DrawSelf(Drawing::RenderContext &context)
	{
		Control::DrawSelf(context);

		if (selected != nullptr && selected->TabPage != nullptr)
		{
			for (int i = startIndex; i < maxIndex; ++i)
			{
				bindings[i]->Button->Render();
			}

			nextSwitchButton->Render();
			lastSwitchButton->Render();

			selected->TabPage->Render();
		}
	}
	//---------------------------------------------------------------------------
	//TabControl::TabControlButton
	//---------------------------------------------------------------------------
	TabControl::TabControlButton::TabControlButton(TabPageButtonBinding &_binding)
		: binding(_binding),
		  active(false)
	{
		SetAutoSize(true);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::SetActive(bool _active)
	{
		active = _active;

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::OnMouseClick(const MouseMessage &mouse)
	{
		Control::OnMouseClick(mouse);

		if (!active)
		{
			if (GetParent() != nullptr)
			{
				static_cast<TabControl*>(GetParent())->SetSelectedIndex(binding.Index);
			}
		}
	}
	//---------------------------------------------------------------------------
	//TabControl::TabControlSwitchButton
	//---------------------------------------------------------------------------
	const Drawing::SizeI TabControl::TabControlSwitchButton::DefaultSize(7, 9);
	//---------------------------------------------------------------------------
	TabControl::TabControlSwitchButton::TabControlSwitchButton(TabControlSwitchButtonDirection direction)
		: direction_(direction)
	{
		SetSize(DefaultSize);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlSwitchButton::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry_);

		g.FillRectangle(GetForeColor(), PointF(0, 0), GetSize());
		g.FillRectangle(GetBackColor(), PointF(1, 1), GetSize() - SizeF(2, 2));

		if (direction_ == TabControlSwitchButtonDirection::Left)
		{
			for (int i = 0; i < 3; ++i)
			{
				g.FillRectangle(GetForeColor(), PointF(2 + i, 4 - i), SizeF(1, 1 + i * 2));
			}
		}
		else
		{
			for (int i = 0; i < 3; ++i)
			{
				g.FillRectangle(GetForeColor(), PointF(2 + i, 2 + i), SizeF(1, 5 - i * 2));
			}
		}
	}
	//---------------------------------------------------------------------------
}
