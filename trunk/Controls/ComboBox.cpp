/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "ComboBox.hpp"
#include "ListBox.hpp"
#include "ScrollBar.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ComboBox::ComboBox()
	{
		type = CONTROL_COMBOBOX;
		
		droppedDown = false;

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
			switch (args.KeyCode)
			{
				case Key::Up:
				case Key::Down:
				case Key::Home:
				case Key::End:
				case Key::PageUp:
				case Key::PageDown:
				{
					int newSelectedIndex = listBox->GetSelectedIndex();

					switch (args.KeyCode)
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
			if (newFocusedControl->GetParent() == this || newFocusedControl->GetParent()->GetParent() == this)
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
		listBox->GetSelectedIndexChangedEvent() += SelectedIndexChangedEventHandler([this](Control*)
		{
			Collapse();
			button->SetText(listBox->GetSelectedItem());
		});
		listBox->GetFocusLostEvent() += FocusLostEventHandler([this](Control*, Control *newFocusedControl)
		{
			if (newFocusedControl == 0 || newFocusedControl->GetParent() == this || newFocusedControl->GetParent()->GetParent() == this)
			{
				return;
			}
			Collapse();
		});
		AddSubControl(listBox);

		SetSize(160, 24);
		
		SetBackColor(Drawing::Color(0xFF4E4E4E));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	ComboBox::~ComboBox()
	{
		Clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ComboBox::SetSize(const Drawing::Size &size)
	{
		ContainerControl::SetSize(size);

		button->SetSize(size);
		listBox->SetLocation(0, button->GetBottom() + 2);
		listBox->SetSize(size.Width, listBox->GetHeight());
	}
	//---------------------------------------------------------------------------
	void ComboBox::SetFont(const std::shared_ptr<Drawing::IFont> &font)
	{
		ContainerControl::SetFont(font);

		button->SetFont(font);
		listBox->SetLocation(0, button->GetBottom() + 2);
		listBox->SetFont(font);
	}
	//---------------------------------------------------------------------------
	bool ComboBox::GetIsFocused() const
	{
		return ContainerControl::GetIsFocused() || button->GetIsFocused() || listBox->GetIsFocused();
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
	}
	//---------------------------------------------------------------------------
	void ComboBox::Collapse()
	{
		droppedDown = false;
		listBox->SetVisible(false);

		Focus();
	}
	//---------------------------------------------------------------------------
	void ComboBox::AddItem(const Misc::AnsiString &text)
	{
		int newHeight = MaxListBoxHeight;
		if (listBox->GetHeight() + font->GetSize() + 2 < MaxListBoxHeight)
		{
			newHeight = listBox->GetHeight() + font->GetSize() + 4;
		}
		listBox->SetSize(listBox->GetWidth(), newHeight);

		listBox->AddItem(text);
	}
	//---------------------------------------------------------------------------
	void ComboBox::InsertItem(int index, const Misc::AnsiString &text)
	{
		int newHeight = MaxListBoxHeight;
		if (listBox->GetHeight() + font->GetSize() + 2 < MaxListBoxHeight)
		{
			newHeight = listBox->GetHeight() + font->GetSize() + 3;
		}
		listBox->SetSize(listBox->GetWidth(), newHeight);

		listBox->InsertItem(index, text);
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
	bool ComboBox::Intersect(const Drawing::Point &point) const
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
	//Event-Handling
	//---------------------------------------------------------------------------
	void ComboBox::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		button->Render(renderer);
		
		if (droppedDown)
		{
			listBox->Render(renderer);
		}
	}
	//---------------------------------------------------------------------------
	//ComboBox::ComboBoxButton
	//---------------------------------------------------------------------------
	void ComboBox::ComboBoxButton::SetSize(const Drawing::Size &size)
	{
		realSize = size;

		Button::SetSize(size.InflateEx(-24, 0));
	}
	//---------------------------------------------------------------------------
	bool ComboBox::ComboBoxButton::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, realSize, point);
	}
	//---------------------------------------------------------------------------
	bool ComboBox::ComboBoxButton::OnKeyDown(const KeyboardMessage &keyboard)
	{
		return Control::OnKeyDown(keyboard);
	}
	//---------------------------------------------------------------------------
	void ComboBox::ComboBoxButton::Render(Drawing::IRenderer *renderer)
	{
		Drawing::Color tempColor = backColor;
		if (isFocused || isInside)
		{
			tempColor += mouseOverFocusColor;
		}
		
		renderer->SetRenderColor(tempColor);
		renderer->Fill(absoluteLocation.Left, absoluteLocation.Top + 1, realSize.Width, realSize.Height - 2);
		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top, realSize.Width - 2, realSize.Height);
		renderer->FillGradient(absoluteLocation.Left + 1, absoluteLocation.Top + 1, realSize.Width - 2, realSize.Height - 2, tempColor - Drawing::Color(0, 20, 20, 20));

		label->Render(renderer);

		int arrowLeft = absoluteLocation.Left + realSize.Width - 9;
		int arrowTop = absoluteLocation.Top + realSize.Height - 11;
		for (int i = 0; i < 4; ++i)
		{
			renderer->Fill(arrowLeft - i, arrowTop - i, 1 + i * 2, 1);
		}
	}
	//---------------------------------------------------------------------------
}