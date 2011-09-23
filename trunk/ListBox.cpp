#include "ListBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ListBox::ListBox(Control *parent) : Control(parent), scrollBar(this)
	{
		type = CONTROL_LISTBOX;
		
		selectedIndex = 0;
		firstVisibleItemIndex = 0;
		drag = false;

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
	const Misc::UnicodeString& ListBox::GetItem(int index) const
	{
		if (index < 0 || index >= (int)items.size())
		{
			throw 1;
		}

		return items.at(index);
	}
	//---------------------------------------------------------------------------
	int ListBox::GetSelectedIndex() const
	{
		return selectedIndex;
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& ListBox::GetSelectedItem() const
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
	bool ListBox::CanHaveFocus() const
	{
		return visible && enabled;
	}
	//---------------------------------------------------------------------------
	bool ListBox::ContainsPoint(const Drawing::Point &point) const
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void ListBox::Invalidate()
	{
		clientArea = bounds;
		
		scrollBar.Invalidate();

		itemsRect = Drawing::Rectangle(clientArea.GetLeft() + 4, clientArea.GetTop() + 4, clientArea.GetWidth() - (scrollBar.GetVisible() ? scrollBar.GetWidth() : 0) - 8, clientArea.GetHeight() - 8);

		scrollBar.SetPageSize(itemsRect.GetHeight() / (font->GetSize() + 2));
		if (scrollBar.ShowItem(selectedIndex))
		{
			firstVisibleItemIndex = scrollBar.GetPosition();
		}
	}
	//---------------------------------------------------------------------------
	bool ListBox::AddItem(const Misc::UnicodeString &text)
	{	
		return InsertItem(items.size() > 0 ? items.size() : 0, text);
	}
	//---------------------------------------------------------------------------
	bool ListBox::InsertItem(int index, const Misc::UnicodeString &text)
	{			
		items.insert(items.begin() + index, text);

		scrollBar.SetRange(items.size());

		Invalidate();

		return true;
	}
	//---------------------------------------------------------------------------
	bool ListBox::RemoveItem(int index)
	{
		if (index < 0 || index >= (int)items.size())
		{
			return false;
		}
		
		items.erase(items.begin() + index);

		scrollBar.SetRange(items.size());
		if (selectedIndex >= (int)items.size())
		{
			selectedIndex = items.size() - 1;
			
			selectedIndexChangedEvent.Invoke(this);
		}

		Invalidate();

		return true;
	}
	//---------------------------------------------------------------------------
	bool ListBox::Clear()
	{
		items.clear();
		
		scrollBar.SetRange(1);
		
		selectedIndex = -1;

		Invalidate();

		return true;
	}
	//---------------------------------------------------------------------------
	void ListBox::SelectItem(int index)
	{
		if (items.size() == 0)
		{
			return;
		}

		int oldSelectedIndex = selectedIndex;

		selectedIndex = index;

		if (selectedIndex < 0)
		{
			selectedIndex = 0;
		}
		if (selectedIndex >= (int)items.size())
		{
			selectedIndex = items.size() - 1;
		}

		if (oldSelectedIndex != selectedIndex)
		{
			if (scrollBar.ShowItem(selectedIndex))
			{
				firstVisibleItemIndex = scrollBar.GetPosition();
			}
			
			selectedIndexChangedEvent.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes ListBox::ProcessEvent(Event *event)
	{
		if (event == 0)
		{
			return Event::DontContinue;
		}

		if (!visible || !enabled)
		{
			return Event::Continue;
		}

		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			if (mouse->State == MouseEvent::LeftDown)
			{
				if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position)) //clientArea
				{
					if (!hasFocus)
					{
						Parent->RequestFocus(this);
					}
				}
				else
				{
					mouse->Position = mousePositionBackup;

					return Event::Continue;
				}
			}
			else if (mouse->State == MouseEvent::Scroll)
			{
				if (!hasFocus)
				{
					mouse->Position = mousePositionBackup;

					return Event::Continue;
				}
			}
		}
		
		if (scrollBar.ProcessEvent(event) == Event::DontContinue)
		{
			firstVisibleItemIndex = scrollBar.GetPosition();
		
			return Event::DontContinue;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			
			if (mouse->State == MouseEvent::LeftDown)
			{
				if (items.size() != 0 && Drawing::Rectangle(4, 4, clientArea.GetWidth() - (scrollBar.GetVisible() ? scrollBar.GetWidth() : 0) - 8, clientArea.GetHeight() - 8).Contains(mouse->Position)) //itemsRect
				{	
					int itemIndex = -1;
					for (unsigned int i = 0; (int)i < itemsRect.GetHeight() / (font->GetSize() + 2) && i < items.size(); ++i)
					{
						if ((int)i * (font->GetSize() + 2) + (font->GetSize() + 2) > mouse->Position.Y - (itemsRect.GetTop() - bounds.GetTop()))
						{
							itemIndex = i;
							break;
						}
					}

					if (itemIndex != -1)
					{
						selectedIndex = itemIndex + firstVisibleItemIndex;
						selectedIndexChangedEvent.Invoke(this);
					}
				}

				return Event::DontContinue;
			}
		}
		else if (event->Type == Event::Keyboard)
		{
			if (items.size() == 0)
			{
				return Event::DontContinue;
			}
		
			KeyboardEvent *keyboard = (KeyboardEvent*)event;
			if (keyboard->State == KeyboardEvent::Down)
			{
				switch (keyboard->KeyCode)
				{
					case Key::Up:
					case Key::Down:
					case Key::Home:
					case Key::End:
					case Key::PageUp:
					case Key::PageDown:
						int oldSelectedIndex = selectedIndex;

						switch (keyboard->KeyCode)
						{
							case Key::Up:
								--selectedIndex;
								break;
							case Key::Down:
								++selectedIndex;
								break;
							case Key::Home:
								selectedIndex = 0;
								break;
							case Key::End:
								selectedIndex = items.size() - 1;
								break;
							case Key::PageUp:
								selectedIndex += scrollBar.GetPageSize() - 1;
								break;
							case Key::PageDown:
								selectedIndex -= scrollBar.GetPageSize() - 1;
								break;
						}

						if (selectedIndex < 0)
						{
							selectedIndex = 0;
						}
						if (selectedIndex >= (int)items.size())
						{
							selectedIndex = items.size() - 1;
						}

						if (oldSelectedIndex != selectedIndex)
						{
							if (scrollBar.ShowItem(selectedIndex))
							{
								firstVisibleItemIndex = scrollBar.GetPosition();
							}
						
							selectedIndexChangedEvent.Invoke(this);
						}
						return Event::DontContinue;
				}
			}
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void ListBox::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(clientArea);
		
		renderer->SetRenderColor(backColor + Drawing::Color(0, 54, 53, 52));
		renderer->Fill(clientArea.GetLeft() + 1, clientArea.GetTop(), clientArea.GetWidth() - 2, 1);
		renderer->Fill(clientArea.GetLeft(), clientArea.GetTop() + 1, 1, clientArea.GetHeight() - 2);
		renderer->Fill(clientArea.GetRight() - 1, clientArea.GetTop() + 1, 1, clientArea.GetHeight() - 2);
		renderer->Fill(clientArea.GetLeft() + 1, clientArea.GetBottom() - 1, clientArea.GetWidth() - 2, 1);

		renderer->SetRenderColor(foreColor);
		for (unsigned int i = 0; (int)i < itemsRect.GetHeight() / (font->GetSize() + 2) && i + firstVisibleItemIndex < items.size(); ++i)
		{
			if (firstVisibleItemIndex + i == selectedIndex)
			{
				renderer->SetRenderColor(Drawing::Color::Black());
				renderer->Fill(itemsRect.GetLeft() - 1, itemsRect.GetTop() + i * (font->GetSize() + 2) - 1, itemsRect.GetWidth() + 2, font->GetSize() + 4);
				renderer->SetRenderColor(foreColor);
			}

			renderer->RenderText(font, itemsRect.GetLeft(), itemsRect.GetTop() + i * (font->GetSize() + 2), itemsRect.GetWidth(), font->GetSize() + 2, items[firstVisibleItemIndex + i]);
		}
	
		scrollBar.Render(renderer);

		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea + bounds.GetPosition() + renderRect.GetPosition());
		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}