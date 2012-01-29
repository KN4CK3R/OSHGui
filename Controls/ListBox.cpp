/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2011 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include <locale>
#include "ListBox.hpp"
#include "ScrollBar.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::Size ListBox::DefaultSize(120, 95);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ListBox::ListBox()
	{
		type = CONTROL_LISTBOX;
		
		selectedIndex = -1;
		firstVisibleItemIndex = 0;
		
		maxVisibleItems = GetHeight() / (font->GetSize() + 2);

		scrollBar = new ScrollBar();
		scrollBar->SetVisible(false);
		scrollBar->GetScrollEvent() += ScrollEventHandler([this](Control*, ScrollEventArgs &args)
		{
			firstVisibleItemIndex = args.NewValue;
		});
		scrollBar->GetFocusLostEvent() += FocusLostEventHandler([this](Control*, Control *newFocusedControl)
		{
			if (newFocusedControl != this)
			{
				OnLostFocus(newFocusedControl);
			}
		});
		AddSubControl(scrollBar);

		SetSize(DefaultSize);

		SetBackColor(Drawing::Color(0xFF171614));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	ListBox::~ListBox()
	{
		Clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ListBox::SetSize(const Drawing::Size &size)
	{
		ContainerControl::SetSize(size);

		itemAreaSize = size.InflateEx(-8, -8);
		if (scrollBar->GetVisible())
		{
			itemAreaSize.Width -= scrollBar->GetWidth();
		}

		maxVisibleItems = std::max(1, itemAreaSize.Height / (font->GetSize() + 2));;

		scrollBar->SetLocation(size.Width - scrollBar->GetWidth(), 0);
		scrollBar->SetSize(scrollBar->GetWidth(), size.Height);
	}
	//---------------------------------------------------------------------------
	void ListBox::SetFont(const std::shared_ptr<Drawing::IFont> &font)
	{
		ContainerControl::SetFont(font);

		maxVisibleItems = std::max(1, itemAreaSize.Height / (font->GetSize() + 2));
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& ListBox::GetItem(int index) const
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (index < 0 || index >= (int)items.size())
		{
			throw Misc::ArgumentOutOfRangeException("index", __FILE__, __LINE__);
		}
		#endif

		return items[index];
	}
	//---------------------------------------------------------------------------
	void ListBox::SetSelectedIndex(int index)
	{
		if (selectedIndex == index)
		{
			return;
		}

		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (index < 0 || index >= (int)items.size())
		{
			throw Misc::ArgumentOutOfRangeException("index", __FILE__, __LINE__);
		}
		#endif

		selectedIndex = index;

		selectedIndexChangedEvent.Invoke(this);

		if (index - firstVisibleItemIndex >= maxVisibleItems || index - firstVisibleItemIndex < 0)
		{
			for (firstVisibleItemIndex = 0; firstVisibleItemIndex <= index; firstVisibleItemIndex += maxVisibleItems);
			firstVisibleItemIndex -= maxVisibleItems;
			if (firstVisibleItemIndex < 0)
			{
				firstVisibleItemIndex = 0;
			}
			scrollBar->SetValue(firstVisibleItemIndex);
		}
	}
	//---------------------------------------------------------------------------
	int ListBox::GetSelectedIndex() const
	{
		return selectedIndex;
	}
	//---------------------------------------------------------------------------
	void ListBox::SetSelectedItem(const Misc::AnsiString &item)
	{
		for (int i = items.size() - 1; i >= 0; --i)
		{
			if (items[i] == item)
			{
				SetSelectedIndex(i);
				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& ListBox::GetSelectedItem() const
	{
		return GetItem(GetSelectedIndex());
	}
	//---------------------------------------------------------------------------
	int ListBox::GetItemsCount() const
	{
		return items.size();
	}
	//---------------------------------------------------------------------------
	SelectedIndexChangedEvent& ListBox::GetSelectedIndexChangedEvent()
	{
		return selectedIndexChangedEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ListBox::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void ListBox::AddItem(const Misc::AnsiString &text)
	{
		InsertItem(items.size() > 0 ? items.size() : 0, text);
	}
	//---------------------------------------------------------------------------
	void ListBox::InsertItem(int index, const Misc::AnsiString &text)
	{
		items.insert(items.begin() + index, text);

		if (items.size() * (font->GetSize() + 2) > itemAreaSize.Height)
		{
			if (!scrollBar->GetVisible())
			{
				itemAreaSize.Width -= scrollBar->GetWidth();
			}
			scrollBar->SetMaximum(items.size() - maxVisibleItems);
			scrollBar->SetVisible(true);
		}
		else if (scrollBar->GetVisible())
		{
			scrollBar->SetVisible(false);
			itemAreaSize.Width += scrollBar->GetWidth();
		}
	}
	//---------------------------------------------------------------------------
	void ListBox::RemoveItem(int index)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (index < 0 || index >= (int)items.size())
		{
			throw Misc::ArgumentOutOfRangeException("index", __FILE__, __LINE__);
		}
		#endif
		
		items.erase(items.begin() + index);

		if (scrollBar->GetVisible())
		{
			scrollBar->SetMaximum(items.size() - maxVisibleItems);
		}
		if (selectedIndex >= (int)items.size())
		{
			selectedIndex = items.size() - 1;
			
			selectedIndexChangedEvent.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
	void ListBox::Clear()
	{
		items.clear();
		
		scrollBar->SetMaximum(1);
		
		selectedIndex = -1;
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void ListBox::OnMouseClick(const MouseMessage &mouse)
	{
		ContainerControl::OnMouseClick(mouse);

		if (Intersection::TestRectangle(absoluteLocation.OffsetEx(4, 4), itemAreaSize, mouse.Location))
		{
			int clickedIndex = firstVisibleItemIndex + (mouse.Location.Y - absoluteLocation.Y - 4) / (font->GetSize() + 2);
			if (clickedIndex < items.size())
			{
				SetSelectedIndex(clickedIndex);
			}
		}
	}
	//---------------------------------------------------------------------------
	void ListBox::OnMouseScroll(const MouseMessage &mouse)
	{
		ContainerControl::OnMouseScroll(mouse);

		int newScrollValue = scrollBar->GetValue() + mouse.Delta;
		if (newScrollValue < 0)
		{
			newScrollValue = 0;
		}
		else if (newScrollValue > items.size() - maxVisibleItems)
		{
			newScrollValue = items.size() - maxVisibleItems;
		}
		scrollBar->SetValue(newScrollValue);
	}
	//---------------------------------------------------------------------------
	bool ListBox::OnKeyDown(const KeyboardMessage &keyboard)
	{
		if (!ContainerControl::OnKeyDown(keyboard))
		{
			switch (keyboard.KeyCode)
			{
				case Key::Up:
				case Key::Down:
				case Key::Home:
				case Key::End:
				case Key::PageUp:
				case Key::PageDown:
				{
					int newSelectedIndex = selectedIndex;

					switch (keyboard.KeyCode)
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
							newSelectedIndex = items.size() - 1;
							break;
						case Key::PageUp:
							newSelectedIndex += maxVisibleItems;
							break;
						case Key::PageDown:
							newSelectedIndex -= maxVisibleItems;
							break;
					}

					if (newSelectedIndex < 0)
					{
						newSelectedIndex = 0;
					}
					if (newSelectedIndex >= (int)items.size())
					{
						newSelectedIndex = items.size() - 1;
					}

					SetSelectedIndex(newSelectedIndex);
				}
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
	bool ListBox::OnKeyPress(const KeyboardMessage &keyboard)
	{
		if (!ContainerControl::OnKeyPress(keyboard))
		{
			if (keyboard.IsAlphaNumeric())
			{
				std::locale loc;
				Misc::AnsiChar keyChar = std::tolower(keyboard.KeyChar, loc);
				int foundIndex = 0;
				for (std::vector<Misc::AnsiString>::iterator it = items.begin(); it != items.end(); ++it, ++foundIndex)
				{
					Misc::AnsiChar check = std::tolower((*it)[0], loc);
					if (check == keyChar && foundIndex != selectedIndex)
					{
						break;
					}
				}
					
				if (foundIndex < (int)items.size())
				{
					SetSelectedIndex(foundIndex);
				}
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
	void ListBox::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top + 1, size.Width - 2, size.Height - 2);
		
		renderer->SetRenderColor(backColor + Drawing::Color(0, 54, 53, 52));
		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top, size.Width - 2, 1);
		renderer->Fill(absoluteLocation.Left, absoluteLocation.Top + 1, 1, size.Height - 2);
		renderer->Fill(absoluteLocation.Left + size.Width - 1, absoluteLocation.Top + 1, 1, size.Height - 2);
		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top + size.Height - 1, size.Width - 2, 1);

		renderer->SetRenderColor(foreColor);
		int itemX = absoluteLocation.Left + 4;
		int itemY = absoluteLocation.Top + 4;
		int padding = font->GetSize() + 2;
		for (int i = 0; i < maxVisibleItems && i + firstVisibleItemIndex < (int)items.size(); ++i)
		{
			if (firstVisibleItemIndex + i == selectedIndex)
			{
				renderer->SetRenderColor(Drawing::Color::Red());
				renderer->Fill(itemX - 1, itemY + i * padding - 1, itemAreaSize.Width + 2, padding);
				renderer->SetRenderColor(foreColor);
			}

			renderer->RenderText(font, itemX, itemY + i * padding, itemAreaSize.Width, padding, items[firstVisibleItemIndex + i]);
		}
	
		scrollBar->Render(renderer);
	}
	//---------------------------------------------------------------------------
}