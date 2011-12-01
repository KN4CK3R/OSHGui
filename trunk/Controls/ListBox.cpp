#include "ListBox.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ListBox::ListBox() : Control(), scrollBar(this)
	{
		type = CONTROL_LISTBOX;
		
		selectedIndex = 0;
		firstVisibleItemIndex = 0;
		drag = false;
		
		SetBounds(6, 6, 120, 95);

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
	int ListBox::GetSelectedIndex() const
	{
		return selectedIndex;
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
	KeyDownEvent& ListBox::GetKeyDownEvent()
	{
		return keyDownEvent;
	}
	//---------------------------------------------------------------------------
	KeyPressEvent& ListBox::GetKeyPressEvent()
	{
		return keyPressEvent;
	}
	//---------------------------------------------------------------------------
	KeyUpEvent& ListBox::GetKeyUpEvent()
	{
		return keyUpEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ListBox::CanHaveFocus() const
	{
		return isVisible && isEnabled;
	}
	//---------------------------------------------------------------------------
	bool ListBox::Intersect(const Drawing::Point &point) const
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
	void ListBox::AddItem(const Misc::AnsiString &text)
	{
		InsertItem(items.size() > 0 ? items.size() : 0, text);
	}
	//---------------------------------------------------------------------------
	void ListBox::InsertItem(int index, const Misc::AnsiString &text)
	{
		items.insert(items.begin() + index, text);

		scrollBar.SetRange(items.size());

		Invalidate();
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

		scrollBar.SetRange(items.size());
		if (selectedIndex >= (int)items.size())
		{
			selectedIndex = items.size() - 1;
			
			selectedIndexChangedEvent.Invoke(this);
		}

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void ListBox::Clear()
	{
		items.clear();
		
		scrollBar.SetRange(1);
		
		selectedIndex = -1;

		Invalidate();
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
	bool ListBox::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException("event", __FILE__, __LINE__);
		}

		if (!isVisible || !isEnabled)
		{
			return false;
		}

		if (event->Type == IEvent::Mouse)
		{
			MouseMessage *mouse = (MouseMessage*)event;
			Drawing::Point mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);

			if (mouse->State == MouseMessage::LeftDown)
			{
				if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position)) //clientArea
				{
					if (!isFocused)
					{
						parent->RequestFocus(this);
					}

					MouseEventArgs args(mouse);
					mouseDownEvent.Invoke(this, args);
				}
				else
				{
					mouse->Position = mousePositionBackup;

					return false;
				}
			}
			else if (mouse->State == MouseMessage::Scroll)
			{
				if (!isFocused)
				{
					mouse->Position = mousePositionBackup;

					return false;
				}
			}
		}
		
		if (scrollBar.ProcessEvent(event) == true)
		{
			firstVisibleItemIndex = scrollBar.GetPosition();
		
			return true;
		}
	
		if (event->Type == IEvent::Mouse)
		{
			MouseMessage *mouse = (MouseMessage*)event;
			
			if (mouse->State == MouseMessage::LeftDown)
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

					MouseEventArgs args(mouse);
					mouseDownEvent.Invoke(this, args);
				}

				return true;
			}
			else if (Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight()).Contains(mouse->Position))
			{
				if (mouse->State == MouseMessage::Move)
				{
					MouseEventArgs args(mouse);
					mouseMoveEvent.Invoke(this, args);

					return true;
				}
				else if (mouse->State == MouseMessage::LeftDown || mouse->State == MouseMessage::RightUp)
				{
					clickEvent.Invoke(this);

					MouseEventArgs args(mouse);
					mouseClickEvent.Invoke(this, args);

					args = MouseEventArgs(mouse);
					mouseUpEvent.Invoke(this, args);

					return true;
				}
			}
		}
		else if (event->Type == IEvent::Keyboard)
		{
			if (items.size() == 0)
			{
				return true;
			}
		
			KeyboardMessage *keyboard = (KeyboardMessage*)event;
			if (keyboard->State == KeyboardMessage::KeyDown)
			{
				KeyEventArgs args(keyboard);
				keyDownEvent.Invoke(this, args);
			
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
				}
			}
			else if (keyboard->State == KeyboardMessage::Character)
			{
				KeyPressEventArgs args(keyboard);
				keyPressEvent.Invoke(this, args);
				if (!args.Handled && keyboard->KeyChar != '\0')
				{
					int foundIndex = 0;
					for (std::vector<Misc::AnsiString>::iterator it = items.begin(); it != items.end(); ++it, ++foundIndex)
					{
						if ((*it)[0] == keyboard->KeyChar && foundIndex != selectedIndex)
						{
							break;
						}
					}
					
					if (foundIndex < (int)items.size())
					{
						selectedIndex = foundIndex;
						
						if (scrollBar.ShowItem(selectedIndex))
						{
							firstVisibleItemIndex = scrollBar.GetPosition();
						}
						
						selectedIndexChangedEvent.Invoke(this);
					}
				}
			}
			else if (keyboard->State == KeyboardMessage::KeyUp)
			{
				KeyEventArgs args(keyboard);
				keyUpEvent.Invoke(this, args);
			}
			
			return true;
		}
		
		return false;
	}
	//---------------------------------------------------------------------------
	void ListBox::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
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