#include "ListBox.h"
#include "Misc\Exceptions.h"

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
	const Misc::UnicodeString& ListBox::GetItem(int index) const
	{
		if (index < 0 || index >= (int)items.size())
		{
			throw ArgumentOutOfRangeException(L"index", __WFILE__, __LINE__);
		}

		return items[index];
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
	void ListBox::AddItem(const Misc::UnicodeString &text)
	{
		InsertItem(items.size() > 0 ? items.size() : 0, text);
	}
	//---------------------------------------------------------------------------
	void ListBox::InsertItem(int index, const Misc::UnicodeString &text)
	{
		items.insert(items.begin() + index, text);

		scrollBar.SetRange(items.size());

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void ListBox::RemoveItem(int index)
	{
		if (index < 0 || index >= (int)items.size())
		{
			throw ArgumentOutOfRangeException(L"index", __WFILE__, __LINE__);
		}
		
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
	IEvent::NextEventTypes ListBox::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw ArgumentNullException(L"event", __WFILE__, __LINE__);
		}

		if (!visible || !enabled)
		{
			return IEvent::Continue;
		}

		if (event->Type == IEvent::Mouse)
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
						parent->RequestFocus(this);
					}

					mouseDownEvent.Invoke(this, MouseEventArgs(mouse));
				}
				else
				{
					mouse->Position = mousePositionBackup;

					return IEvent::Continue;
				}
			}
			else if (mouse->State == MouseEvent::Scroll)
			{
				if (!hasFocus)
				{
					mouse->Position = mousePositionBackup;

					return IEvent::Continue;
				}
			}
		}
		
		if (scrollBar.ProcessEvent(event) == IEvent::DontContinue)
		{
			firstVisibleItemIndex = scrollBar.GetPosition();
		
			return IEvent::DontContinue;
		}
	
		if (event->Type == IEvent::Mouse)
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

					mouseDownEvent.Invoke(this, MouseEventArgs(mouse));
				}

				return IEvent::DontContinue;
			}
			else if (Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight()).Contains(mouse->Position))
			{
				if (mouse->State == MouseEvent::Move)
				{
					mouseMoveEvent.Invoke(this, MouseEventArgs(mouse));

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftDown || mouse->State == MouseEvent::RightUp)
				{
					clickEvent.Invoke(this);

					mouseClickEvent.Invoke(this, MouseEventArgs(mouse));

					mouseUpEvent.Invoke(this, MouseEventArgs(mouse));

					return IEvent::DontContinue;
				}
			}
		}
		else if (event->Type == IEvent::Keyboard)
		{
			if (items.size() == 0)
			{
				return IEvent::DontContinue;
			}
		
			KeyboardEvent *keyboard = (KeyboardEvent*)event;
			if (keyboard->State == KeyboardEvent::KeyDown)
			{
				keyDownEvent.Invoke(this, KeyEventArgs(keyboard));
			
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
			else if (keyboard->State == KeyboardEvent::Character)
			{
				KeyPressEventArgs args(keyboard);
				keyPressEvent.Invoke(this, args);
				if (!args.Handled && keyboard->KeyChar != L'\0')
				{
					int foundIndex = 0;
					for (std::vector<Misc::UnicodeString>::iterator it = items.begin(); it != items.end(); ++it, ++foundIndex)
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
			else if (keyboard->State == KeyboardEvent::KeyUp)
			{
				keyUpEvent.Invoke(this, KeyEventArgs(keyboard));
			}
			
			return IEvent::DontContinue;
		}
		
		return IEvent::Continue;
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