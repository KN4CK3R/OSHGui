/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include <locale>
#include "ListBox.hpp"
#include "ScrollBar.hpp"
#include "../Misc/Exceptions.hpp"
#include "../Misc/Intersection.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeF ListBox::DefaultSize(120, 95);
	const Drawing::SizeF ListBox::DefaultItemAreaPadding(8, 8);
	const int ListBox::DefaultItemPadding(2);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ListBox::ListBox()
		: selectedIndex(-1),
		  firstVisibleItemIndex(0),
		  autoScrollEnabled(false)
	{
		type = ControlType::ListBox;
	
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
		
		maxVisibleItems = GetHeight() / (GetFont()->GetFontHeight() + DefaultItemPadding);

		ApplyTheme(Application::Instance().GetTheme());
	}
	//---------------------------------------------------------------------------
	ListBox::~ListBox()
	{
		Clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ListBox::SetSize(const Drawing::SizeF &size)
	{
		Control::SetSize(size);

		itemAreaSize = size.InflateEx(-8, -8);
		if (scrollBar->GetVisible())
		{
			itemAreaSize.Width -= scrollBar->GetWidth();
		}

		scrollBar->SetLocation(size.Width - scrollBar->GetWidth() - 1, 0);
		scrollBar->SetSize(scrollBar->GetWidth(), size.Height);

		CheckForScrollBar();
	}
	//---------------------------------------------------------------------------
	void ListBox::SetFont(const Drawing::FontPtr &font)
	{
		Control::SetFont(font);

		CheckForScrollBar();
	}
	//---------------------------------------------------------------------------
	void ListBox::SetAutoScrollEnabled(bool autoScrollEnabled)
	{
		this->autoScrollEnabled = autoScrollEnabled;
	}
	//---------------------------------------------------------------------------
	bool ListBox::GetAutoScrollEnabled() const
	{
		return autoScrollEnabled;
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& ListBox::GetItem(int index) const
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (index < 0 || index >= (int)items.size())
		{
			throw Misc::ArgumentOutOfRangeException("index");
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
			throw Misc::ArgumentOutOfRangeException("index");
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
	bool ListBox::Intersect(const Drawing::PointF &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, scrollBar->GetVisible() ? size.InflateEx(-scrollBar->GetWidth(), 0) : size, point);
	}
	//---------------------------------------------------------------------------
	void ListBox::ExpandSizeToShowItems(int count)
	{
		auto itemHeight = GetFont()->GetFontHeight() + DefaultItemPadding;
		int newHeight = count * itemHeight;

		SetSize(GetWidth(), newHeight + DefaultItemAreaPadding.Height);
	}
	//---------------------------------------------------------------------------
	void ListBox::AddItem(const Misc::AnsiString &text)
	{
		InsertItem(!items.empty() ? items.size() : 0, text);
	}
	//---------------------------------------------------------------------------
	void ListBox::InsertItem(int index, const Misc::AnsiString &text)
	{
		items.insert(items.begin() + index, text);

		CheckForScrollBar();

		if (autoScrollEnabled)
		{
			scrollBar->SetValue(index);
		}
	}
	//---------------------------------------------------------------------------
	void ListBox::RemoveItem(int index)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (index < 0 || index >= (int)items.size())
		{
			throw Misc::ArgumentOutOfRangeException("index");
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

		CheckForScrollBar();
	}
	//---------------------------------------------------------------------------
	void ListBox::CheckForScrollBar()
	{
		auto itemHeight = GetFont()->GetFontHeight() + DefaultItemPadding;

		maxVisibleItems = std::max(1.0f, itemAreaSize.Height / itemHeight);

		if (!items.empty() && items.size() * itemHeight > itemAreaSize.Height)
		{
			if (!scrollBar->GetVisible())
			{
				itemAreaSize.Width -= scrollBar->GetWidth();
			}
			scrollBar->SetMaximum(items.size() - maxVisibleItems - 1);
			scrollBar->SetVisible(true);
		}
		else if (scrollBar->GetVisible())
		{
			scrollBar->SetVisible(false);
			itemAreaSize.Width += scrollBar->GetWidth();
		}
	}
	//---------------------------------------------------------------------------
	void ListBox::DrawSelf(Drawing::RenderContext &context)
	{
		Control::DrawSelf(context);

		scrollBar->Render();
	}
	//---------------------------------------------------------------------------
	void ListBox::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry);

		g.FillRectangle(GetBackColor(), PointF(1, 1), GetSize() - SizeF(2, 2));

		auto color = GetBackColor() + Color(0, 54, 53, 52);
		g.FillRectangle(color, PointF(1, 0), SizeF(GetWidth() - 2, 1));
		g.FillRectangle(color, PointF(0, 1), SizeF(1, GetHeight() - 2));
		g.FillRectangle(color, PointF(GetWidth() - 1, 1), SizeF(1, GetHeight() - 2));
		g.FillRectangle(color, PointF(1, GetHeight() - 1), SizeF(GetWidth() - 2, 1));

		int itemX = 4;
		int itemY = 4;
		int padding = GetFont()->GetFontHeight() + DefaultItemPadding;
		for (int i = 0; i < maxVisibleItems && i + firstVisibleItemIndex < (int)items.size(); ++i)
		{
			if (firstVisibleItemIndex + i == selectedIndex)
			{
				g.FillRectangle(Color::Red(), PointF(itemX - 1, itemY + i * padding - 1), SizeF(itemAreaSize.Width + 2, padding));
			}

			g.DrawString(items[firstVisibleItemIndex + i], GetFont(), GetForeColor(), PointF(itemX, itemY + i * padding));//, SizeF(itemAreaSize.Width, padding));
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void ListBox::OnMouseClick(const MouseMessage &mouse)
	{
		Control::OnMouseClick(mouse);

		if (Intersection::TestRectangle(absoluteLocation.OffsetEx(4, 4), itemAreaSize, mouse.Location))
		{
			int clickedIndex = firstVisibleItemIndex + (mouse.Location.Y - absoluteLocation.Y - 4) / (GetFont()->GetFontHeight() + DefaultItemPadding);
			if (clickedIndex < items.size())
			{
				SetSelectedIndex(clickedIndex);
			}
		}
	}
	//---------------------------------------------------------------------------
	void ListBox::OnMouseScroll(const MouseMessage &mouse)
	{
		Control::OnMouseScroll(mouse);

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
		if (!Control::OnKeyDown(keyboard))
		{
			switch (keyboard.GetKeyCode())
			{
				case Key::Up:
				case Key::Down:
				case Key::Home:
				case Key::End:
				case Key::PageUp:
				case Key::PageDown:
				{
					int newSelectedIndex = selectedIndex;

					switch (keyboard.GetKeyCode())
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

					return true;
				}
			}
		}

		return false;
	}
	//---------------------------------------------------------------------------
	bool ListBox::OnKeyPress(const KeyboardMessage &keyboard)
	{
		if (!Control::OnKeyPress(keyboard))
		{
			if (keyboard.IsAlphaNumeric())
			{
				std::locale loc;
				Misc::AnsiChar keyChar = std::tolower(keyboard.GetKeyChar(), loc);
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
}