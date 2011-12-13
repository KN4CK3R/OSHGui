#include "ListBox.hpp"
#include "..\Misc\Exceptions.hpp"

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
		
		selectedIndex = 0;
		firstVisibleItemIndex = 0;
		
		maxVisibleItems = GetHeight() / (font->GetSize() + 2);

		scrollBar = new ScrollBar();
		scrollBar->SetVisible(false);
		scrollBar->GetScrollEvent() += ScrollEventHandler([this](Control*, ScrollEventArgs &args)
		{
			firstVisibleItemIndex = args.NewValue;
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

		maxVisibleItems = itemAreaSize.Height / (font->GetSize() + 2);

		scrollBar->SetLocation(size.Width - scrollBar->GetWidth(), 0);
		scrollBar->SetSize(scrollBar->GetWidth(), size.Height);
	}
	//---------------------------------------------------------------------------
	void ListBox::SetFont(const std::shared_ptr<Drawing::IFont> &font)
	{
		ContainerControl::SetFont(font);

		maxVisibleItems = itemAreaSize.Height / (font->GetSize() + 2);
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

		scrollBar->SetValue(index);

		selectedIndexChangedEvent.Invoke(this);

		for (firstVisibleItemIndex = 0; firstVisibleItemIndex <= index; firstVisibleItemIndex += maxVisibleItems);
		firstVisibleItemIndex -= maxVisibleItems;
		if (firstVisibleItemIndex < 0)
		{
			firstVisibleItemIndex = 0;
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

		if (scrollBar->GetVisible())
		{
			scrollBar->SetMaximum(items.size() - maxVisibleItems);
		}
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
		
		scrollBar->SetMaximum(1);
		
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
			scrollBar->SetValue(selectedIndex);
			//if (selectedIndex > firstVisibleItemIndex
			//if (scrollBar->ShowItem(selectedIndex))
			{
				//firstVisibleItemIndex = scrollBar->GetPosition();
			}
			
			selectedIndexChangedEvent.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void ListBox::OnMouseClick(const MouseMessage &mouse)
	{
		ContainerControl::OnMouseClick(mouse);

		if (Intersection::TestRectangle(absoluteLocation.OffsetEx(4, 4), itemAreaSize, mouse.Position))
		{

		}
	}
	//---------------------------------------------------------------------------
	bool ListBox::OnKeyPress(const KeyboardMessage &keyboard)
	{
		if (!ContainerControl::OnKeyPress(keyboard))
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
						int oldSelectedIndex = selectedIndex;
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

						if (oldSelectedIndex != newSelectedIndex)
						{
							SetSelectedIndex(newSelectedIndex);
						}
					}
					break;
			}
				/*if (!args.Handled && keyboard->KeyChar != '\0')
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
				}*/
		}
		return true;
	}
	//---------------------------------------------------------------------------
	//---------------------------------------------------------------------------
	/*bool ListBox::ProcessEvent(IEvent *event)
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
	//---------------------------------------------------------------------------*/
	void ListBox::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(absoluteLocation, size);
		
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

		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea + bounds.GetPosition() + renderRect.GetPosition());
		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}