/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
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
	const Drawing::SizeF TabControl::DefaultSize(200, 200);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TabControl::TabControl()
		: startIndex(0),
		  maxIndex(0),
		  selected(nullptr)
	{
		type = ControlType::TabControl;

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
		for (auto &binding : bindings)
		{
			delete binding;
		}
		bindings.clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TabControl::SetSize(const Drawing::SizeF &size)
	{
		Control::SetSize(size);

		CalculateButtonLocationAndCount();

		lastSwitchButton->SetLocation(GetWidth() - TabControlSwitchButton::DefaultSize.Width, 0);
		nextSwitchButton->SetLocation(GetWidth() - TabControlSwitchButton::DefaultSize.Width, TabControlSwitchButton::DefaultSize.Height + 1);

		if (selected != nullptr)
		{
			auto tabPageSize = size.InflateEx(0, -selected->button->GetHeight());

			for (auto &binding : bindings)
			{
				binding->tabPage->SetSize(tabPageSize);
			}
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::SetForeColor(Drawing::Color color)
	{
		Control::SetForeColor(color);

		lastSwitchButton->SetForeColor(color);
		nextSwitchButton->SetForeColor(color);

		for (auto &binding : bindings)
		{
			binding->button->SetForeColor(color);
			binding->tabPage->SetForeColor(color);
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::SetBackColor(Drawing::Color color)
	{
		Control::SetBackColor(color);

		lastSwitchButton->SetBackColor(color);
		nextSwitchButton->SetBackColor(color);

		for (auto &binding : bindings)
		{
			binding->button->SetBackColor(color);
			binding->tabPage->SetBackColor(color);
		}
	}
	//---------------------------------------------------------------------------
	TabPage* TabControl::GetTabPage(const Misc::AnsiString &text) const
	{
		for (auto &binding : bindings)
		{
			if (binding->tabPage->GetText() == text)
			{
				return binding->tabPage;
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
		for (auto &binding : bindings)
		{
			if (binding->index == index)
			{
				SelectBinding(*binding);

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
		for (auto &binding : bindings)
		{
			if (binding->tabPage == tabPage)
			{
				SelectBinding(*binding);

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
			throw Misc::ArgumentNullException("tabPage");
			#endif
			return;
		}

		for (auto &binding : bindings)
		{
			if (binding->tabPage == tabPage)
			{
				return;
			}
		}

		auto binding = new TabPageButtonBinding();
		binding->index = bindings.size();
		binding->tabPage = tabPage;

		auto button = new TabControlButton(*binding);
		button->SetLocation(Drawing::PointF(0, 0));
		button->SetForeColor(GetForeColor());
		button->SetBackColor(GetBackColor());
		button->SetFont(font);

		AddSubControl(button);

		tabPage->SetSize(size.InflateEx(0, -button->GetHeight()));

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
			throw Misc::ArgumentNullException("tabPage");
			#endif
			return;
		}

		for (auto &binding : bindings)
		{
			if (binding->tabPage == tabPage)
			{
				RemoveControl(binding->button);
				RemoveControl(binding->tabPage);

				delete binding->button;
				binding->tabPage->button = nullptr;
				
				bindings.erase(std::remove(std::begin(bindings), std::end(bindings), binding), std::end(bindings));

				if (selected->tabPage == tabPage)
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
	bool TabControl::Intersect(const Drawing::PointF &point) const
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

			int tempWidth = 0;
			int maxWidth = size.Width - TabControlSwitchButton::DefaultSize.Width;
			for (auto &binding : bindings)
			{
				auto button = binding->button;
				button->SetVisible(false);

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

			if (selected != nullptr)
			{
				selected->tabPage->SetLocation(0, selected->button->GetSize().Height);
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
		selected->tabPage->SetVisible(false);
		selected->button->SetActive(false);
		selected = &binding;
		selected->button->SetActive(true);
		selected->tabPage->SetVisible(true);
		CalculateButtonLocationAndCount();

		selectedIndexChangedEvent.Invoke(this);
	}
	//---------------------------------------------------------------------------
	void TabControl::ApplyTheme(const Drawing::Theme &theme)
	{
		ContainerControl::ApplyTheme(theme);
		
		for (auto &binding : bindings)
		{
			binding->tabPage->ApplyTheme(theme);
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::DrawSelf(Drawing::RenderContext &context)
	{
		ContainerControl::DrawSelf(context);

		if (selected->tabPage != nullptr)
		{
			for (int i = startIndex; i < maxIndex; ++i)
			{
				bindings[i]->button->Render();
			}

			nextSwitchButton->Render();
			lastSwitchButton->Render();

			selected->tabPage->Render();
		}
	}
	//---------------------------------------------------------------------------
	//TabControl::TabControlButton
	//---------------------------------------------------------------------------
	const Drawing::PointF TabControl::TabControlButton::DefaultLabelOffset(4, 2);
	//---------------------------------------------------------------------------
	TabControl::TabControlButton::TabControlButton(TabPageButtonBinding &_binding)
		: binding(_binding),
		  label(new Label())
	{
		active = false;

		label->SetLocation(DefaultLabelOffset);
		label->SetText(binding.tabPage->GetText());
		label->SetBackColor(Drawing::Color::Empty());

		size = label->GetSize().InflateEx(DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::SetForeColor(Drawing::Color color)
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

		Invalidate();
	}
	//---------------------------------------------------------------------------
	bool TabControl::TabControlButton::Intersect(const Drawing::PointF &point) const
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
				static_cast<TabControl*>(parent)->SetSelectedIndex(binding.index);
			}
		}
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::DrawSelf(Drawing::RenderContext &context)
	{
		Control::DrawSelf(context);

		label->Render();
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(geometry);

		if (active)
		{
			g.FillRectangleGradient(ColorRectangle(GetBackColor() + Color(0, 43, 43, 43), GetBackColor() - Color(0, 10, 10, 10)), PointF(0, 0), GetSize());
			g.FillRectangleGradient(ColorRectangle(GetBackColor(), GetBackColor() - Color(0, 42, 42, 42)), PointF(1, 1), GetSize() - SizeF(2, 0));
		}
		else
		{
			auto backInactive = (isInside ? GetBackColor() + Color(0, 50, 50, 50) : GetBackColor()) - Color(0, 47, 47, 47);

			g.FillRectangle(backInactive + Color(0, 9, 9, 9), PointF(0, 0), GetSize());
			g.FillRectangleGradient(ColorRectangle(backInactive, backInactive - Color(0, 20, 20, 20)), PointF(1, 1), GetSize() - SizeF(2, 1));
		}
	}
	//---------------------------------------------------------------------------
	//TabControl::TabControlSwitchButton
	//---------------------------------------------------------------------------
	const Drawing::SizeF TabControl::TabControlSwitchButton::DefaultSize(9, 9);
	//---------------------------------------------------------------------------
	TabControl::TabControlSwitchButton::TabControlSwitchButton(int _direction)
		: direction(_direction)
	{
		SetSize(DefaultSize);
	}
	//---------------------------------------------------------------------------
	bool TabControl::TabControlSwitchButton::Intersect(const Drawing::PointF &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void TabControl::TabControlSwitchButton::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(geometry);

		auto base = isInside ? backColor : backColor - Color(0, 47, 47, 47);
		auto borderColor = backColor + Color(0, 9, 9, 9);

		g.FillRectangle(borderColor, PointF(0, 0), GetSize());
		g.FillRectangle(base, PointF(1, 1), GetSize() - SizeF(2, 2));

		if (direction == 0)
		{
			for (int i = 0; i < 3; ++i)
			{
				g.FillRectangle(GetForeColor(), PointF(3 + i, 4 - i), SizeF(1, 1 + i * 2));
			}
		}
		else
		{
			for (int i = 0; i < 3; ++i)
			{
				g.FillRectangle(GetForeColor(), PointF(3 + i, 2 + i), SizeF(1, 5 - i * 2));
			}
		}
	}
	//---------------------------------------------------------------------------
}
