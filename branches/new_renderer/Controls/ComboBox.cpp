/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */
#include "ComboBox.hpp"
#include "ListBox.hpp"
#include "ScrollBar.hpp"
#include "../Misc/Exceptions.hpp"
#include "../Misc/Intersection.hpp"

namespace OSHGui
{
	const int ComboBox::DefaultMaxShowItems(10);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ComboBox::ComboBox()
		: droppedDown(false),
		  maxShowItems(DefaultMaxShowItems)
	{
		type = ControlType::ComboBox;
	
		button = new ComboBoxButton();
		button->SetLocation(0, 0);
		button->GetClickEvent() += ClickEventHandler([this](Control*)
		{
			if (!droppedDown)
			{
				Expand();
			}
			else
			{
				Collapse();
			}
		});
		button->GetKeyDownEvent() += KeyDownEventHandler([this](Control*, KeyEventArgs &args)
		{
			switch (args.GetKeyCode())
			{
				case Key::Up:
				case Key::Down:
				case Key::Home:
				case Key::End:
				case Key::PageUp:
				case Key::PageDown:
				{
					int newSelectedIndex = listBox->GetSelectedIndex();

					switch (args.GetKeyCode())
					{
						case Key::Up:
							--newSelectedIndex;
							break;
						case Key::Down:
							++newSelectedIndex;
							break;
						case Key::Home:
							newSelectedIndex = 0;
							break;
						case Key::End:
							newSelectedIndex = listBox->GetItemsCount() - 1;
							break;
						case Key::PageUp:
							newSelectedIndex += 4;
							break;
						case Key::PageDown:
							newSelectedIndex -= 4;
							break;
					}

					if (newSelectedIndex < 0)
					{
						newSelectedIndex = 0;
					}
					if (newSelectedIndex >= listBox->GetItemsCount())
					{
						newSelectedIndex = listBox->GetItemsCount() - 1;
					}

					listBox->SetSelectedIndex(newSelectedIndex);
				}
			}
			button->Focus();
		});
		button->GetFocusLostEvent() += FocusLostEventHandler([this](Control*, Control *newFocusedControl)
		{
			if (newFocusedControl == 0 || newFocusedControl->GetParent() == this || newFocusedControl->GetParent()->GetParent() == this)
			{
				return;
			}
			Collapse();
		});
		AddSubControl(button);

		listBox = new ListBox();
		listBox->SetLocation(0, button->GetBottom() + 2);
		listBox->SetSize(listBox->GetWidth(), 4);
		listBox->SetVisible(false);
		listBox->ExpandSizeToShowItems(4);
		listBox->GetSelectedIndexChangedEvent() += SelectedIndexChangedEventHandler([this](Control*)
		{
			button->SetText(listBox->GetSelectedItem());
			if (listBox->GetVisible())
			{
				Collapse();
				button->Focus();
			}
		});
		listBox->GetFocusLostEvent() += FocusLostEventHandler([this](Control*, Control *newFocusedControl)
		{
			if (newFocusedControl == nullptr)
			{
				return;
			}
			Control *parent = newFocusedControl->GetParent();
			if (parent == this)
			{
				return;
			}
			parent = parent->GetParent();
			if (parent == this)
			{
				return;
			}
			parent = parent->GetParent();
			if (parent == this)
			{
				return;
			}
			Collapse();
		});
		AddSubControl(listBox);

		SetSize(160, 24);
		
		ApplyTheme(Application::Instance().GetTheme());
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ComboBox::SetSize(const Drawing::SizeF &size)
	{
		Control::SetSize(size);

		button->SetSize(size);
		listBox->SetLocation(0, button->GetBottom() + 2);
		listBox->SetSize(size.Width, listBox->GetHeight());
	}
	//---------------------------------------------------------------------------
	void ComboBox::SetFont(const Drawing::FontPtr &font)
	{
		Control::SetFont(font);

		button->SetFont(font);
		listBox->SetLocation(0, button->GetBottom() + 2);
		listBox->SetFont(font);
	}
	//---------------------------------------------------------------------------
	void ComboBox::SetForeColor(Drawing::Color color)
	{
		Control::SetForeColor(color);

		button->SetForeColor(color);
		listBox->SetForeColor(color);
	}
	//---------------------------------------------------------------------------
	void ComboBox::SetBackColor(Drawing::Color color)
	{
		Control::SetBackColor(color);

		button->SetBackColor(color);
		listBox->SetBackColor(color);
	}
	//---------------------------------------------------------------------------
	void ComboBox::SetText(const Misc::AnsiString &text)
	{
		button->SetText(text);
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& ComboBox::GetText() const
	{
		return button->GetText();
	}
	//---------------------------------------------------------------------------
	bool ComboBox::GetIsFocused() const
	{
		return Control::GetIsFocused() || button->GetIsFocused() || listBox->GetIsFocused();
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& ComboBox::GetItem(int index) const
	{
		return listBox->GetItem(index);
	}
	//---------------------------------------------------------------------------
	void ComboBox::SetSelectedIndex(int index)
	{
		listBox->SetSelectedIndex(index);

		Collapse();
	}
	//---------------------------------------------------------------------------
	int ComboBox::GetSelectedIndex() const
	{
		return listBox->GetSelectedIndex();
	}
	//---------------------------------------------------------------------------
	void ComboBox::SetSelectedItem(const Misc::AnsiString &item)
	{
		listBox->SetSelectedItem(item);

		Collapse();
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& ComboBox::GetSelectedItem() const
	{
		return listBox->GetSelectedItem();
	}
	//---------------------------------------------------------------------------
	int ComboBox::GetItemsCount() const
	{
		return listBox->GetItemsCount();
	}
	//---------------------------------------------------------------------------
	void ComboBox::SetMaxShowItems(int items)
	{
		maxShowItems = items;
	}
	//---------------------------------------------------------------------------
	int ComboBox::GetMaxShowItems() const
	{
		return maxShowItems;
	}
	//---------------------------------------------------------------------------
	SelectedIndexChangedEvent& ComboBox::GetSelectedIndexChangedEvent()
	{
		return listBox->GetSelectedIndexChangedEvent();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void ComboBox::Expand()
	{
		droppedDown = true;
		listBox->SetVisible(true);
		listBox->Focus();

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void ComboBox::Collapse()
	{
		droppedDown = false;
		listBox->SetVisible(false);

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void ComboBox::AddItem(const Misc::AnsiString &text)
	{
		InsertItem(listBox->GetItemsCount() > 0 ? listBox->GetItemsCount() : 0, text);
	}
	//---------------------------------------------------------------------------
	void ComboBox::InsertItem(int index, const Misc::AnsiString &text)
	{
		listBox->InsertItem(index, text);

		listBox->ExpandSizeToShowItems(std::min(listBox->GetItemsCount(), maxShowItems));
	}
	//---------------------------------------------------------------------------
	void ComboBox::RemoveItem(int index)
	{
		listBox->RemoveItem(index);
	}
	//---------------------------------------------------------------------------
	void ComboBox::Clear()
	{
		Collapse();

		listBox->Clear();
	}
	//---------------------------------------------------------------------------
	bool ComboBox::Intersect(const Drawing::PointF &point) const
	{
		if (Intersection::TestRectangle(absoluteLocation, size, point))
		{
			return true;
		}
		if (droppedDown && listBox->Intersect(point))
		{
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------
	void ComboBox::Focus()
	{
		button->Focus();
	}
	//---------------------------------------------------------------------------
	void ComboBox::DrawSelf(Drawing::RenderContext &context)
	{
		button->Render();

		if (droppedDown)
		{
			listBox->Render();
		}
	}
	//---------------------------------------------------------------------------
	//ComboBox::ComboBoxButton
	//---------------------------------------------------------------------------
	void ComboBox::ComboBoxButton::SetSize(const Drawing::SizeF &size)
	{
		realSize = size;

		Button::SetSize(size.InflateEx(-24, 0));
	}
	//---------------------------------------------------------------------------
	void ComboBox::ComboBoxButton::CalculateLabelLocation()
	{
		label->SetLocation(Drawing::PointF(6, GetSize().Height / 2 - label->GetSize().Height / 2));
	}
	//---------------------------------------------------------------------------
	bool ComboBox::ComboBoxButton::OnKeyDown(const KeyboardMessage &keyboard)
	{
		return Control::OnKeyDown(keyboard);
	}
	//---------------------------------------------------------------------------
	void ComboBox::ComboBoxButton::DrawSelf(Drawing::RenderContext &context)
	{
		Control::DrawSelf(context);

		label->Render();
	}
	//---------------------------------------------------------------------------
	void ComboBox::ComboBoxButton::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry);

		auto color = isFocused || isInside ? GetBackColor() + GetMouseOverFocusColor() : GetBackColor();

		g.FillRectangle(color, PointF(0, 1), realSize - SizeF(0, 2));
		g.FillRectangle(color, PointF(1, 0), realSize - SizeF(2, 0));
		g.FillRectangleGradient(ColorRectangle(color, color - Color(0, 20, 20, 20)), PointF(1, 1), realSize - SizeF(2, 2));

		int arrowLeft = realSize.Width - 9;
		int arrowTop = realSize.Height - 11;
		for (int i = 0; i < 4; ++i)
		{
			g.FillRectangle(GetForeColor(), PointF(arrowLeft - i, arrowTop - i), SizeF(1 + i * 2, 1));
		}
	}
	//---------------------------------------------------------------------------
}
