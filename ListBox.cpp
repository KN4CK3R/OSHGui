#include "ListBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ListBox::ListBox(Panel *parentPanel) : scrollBar(parentPanel)
	{
		type = CONTROL_LISTBOX;
		
		ParentPanel = parentPanel;
		
		selectedIndex = -1;
		drag = false;
	}
	//---------------------------------------------------------------------------
	ListBox::~ListBox()
	{
		Clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	ListItem* ListBox::GetItem(int index)
	{
		if (index < 0 || index >= items.GetSize())
		{
			return NULL;
		}

		return items.Get(index);
	}
	//---------------------------------------------------------------------------
	int ListBox::GetSelectedIndex()
	{
		return selectedIndex;
	}
	//---------------------------------------------------------------------------
	ListItem* ListBox::GetSelectedItem()
	{
		return GetItem(GetSelectedIndex());
	}
	//---------------------------------------------------------------------------
	int ListBox::GetItemsCount()
	{
		return items.GetSize();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ListBox::CanHaveFocus()
	{
		return visible && enabled;
	}
	//---------------------------------------------------------------------------
	bool ListBox::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void ListBox::UpdateRects()
	{
		itemsRect = bounds;
		itemsRect.Offset(4, 4);
		itemsRect.Inflate(-4 - scrollBar.GetBounds().GetWidth(), -4);
		
		scrollBar.SetLocation(Drawing::Point(itemsRect.GetRight(), bounds.GetTop()));
		scrollBar.SetSize(Drawing::Size(-1, bounds.GetHeight()));
		scrollBar.SetPageSize(itemsRect.GetHeight() / LISTBOX_ITEM_HEIGHT);
		scrollBar.ShowItem(selectedIndex);
	}
	//---------------------------------------------------------------------------
	bool ListBox::AddItem(const char *text)
	{
		if (text == NULL)
		{
			return false;
		}
	
		ListItem *newItem = new ListItem();
		if (newItem == NULL)
		{
			return false;
		}
		
		memset((void*)newItem, 0x00, sizeof(ListItem));
		
		strcpy_s(newItem->Text, 256, text);
		
		newItem->ItemRect = Drawing::Rectangle(0, 0, 0, 0);

		if (!items.Add(newItem))
		{
			delete newItem;
			newItem = NULL;
			return false;
		}
		else
		{
			scrollBar.SetRange(0, items.GetSize());
			return true;
		}
	}
	//---------------------------------------------------------------------------
	bool ListBox::InsertItem(int index, const char *text)
	{
		if (text == NULL)
		{
			return false;
		}
	
		ListItem *newItem = new ListItem();
		if (newItem == NULL)
		{
			return false;
		}
		
		memset((void*)newItem, 0x00, sizeof(ListItem));

		strcpy_s(newItem->Text, 256, text);
		
		newItem->ItemRect = Drawing::Rectangle(0, 0, 0, 0);

		if (!items.Insert(index, newItem))
		{
			delete newItem;
			newItem = NULL;
			return false;
		}
		else
		{
			scrollBar.SetRange(0, items.GetSize());
			return true;
		}
	}
	//---------------------------------------------------------------------------
	bool ListBox::RemoveItem(int index)
	{
		if (index < 0 || index >= items.GetSize())
		{
			return false;
		}

		ListItem *item = items.Get(index);
		delete item;
		item = NULL;
		
		items.Remove(index);
		scrollBar.SetRange(0, items.GetSize());
		if (selectedIndex >= items.GetSize())
		{
			selectedIndex = items.GetSize() - 1;
			
			if (changeFunc != NULL)
			{
				(*changeFunc)(this);
			}
		}

		return true;
	}
	//---------------------------------------------------------------------------
	bool ListBox::Clear()
	{
		for (int i = 0; i < items.GetSize(); i++)
		{
			ListItem *item = items.Get(i);
			delete item;
			item = NULL;
		}

		items.Clear();
		
		scrollBar.SetRange(0, 1);
		
		selectedIndex = -1;

		return true;
	}
	//---------------------------------------------------------------------------
	void ListBox::SelectItem(int newIndex)
	{
		if (items.IsEmpty())
		{
			return;
		}

		int oldSelectedIndex = selectedIndex;

		selectedIndex = newIndex;

		if (selectedIndex < 0)
		{
			selectedIndex = 0;
		}
		if (selectedIndex >= items.GetSize())
		{
			selectedIndex = items.GetSize() - 1;
		}

		if (oldSelectedIndex != selectedIndex)
		{
			scrollBar.ShowItem(selectedIndex);
			
			if (changeFunc != NULL)
			{
				(*changeFunc)(this);
			}
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes ListBox::ProcessEvent(Event *event)
	{
		if (event == NULL || !visible || !enabled)
		{
			return Event::None;
		}
		
		if (!hasFocus && event->Type == Event::Mouse && ((MouseEvent*)event)->State == MouseEvent::LeftDown)
		{
			ParentPanel->RequestFocus(this);
		}
		
		if (scrollBar.ProcessEvent(event) == Event::None)
		{
			return Event::None;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*) event;
			
			if (!items.IsEmpty() && scrollBarRect.Contains(mouse->Position))
			{
				if (mouse->State == MouseEvent::LeftDown)
				{
					int itemIndex = scrollBar.GetPosition() + (mouse->Position.Y - itemsRect.GetTop()) / 12;

					if (itemIndex >= scrollBar.GetPosition() && itemIndex < items.GetSize() && itemIndex < scrollBar.GetPosition() + scrollBar.GetPageSize())
					{
						drag = true;

						selectedIndex = itemIndex;
					}
				}
				else if (mouse->State == MouseEvent::Wheel)
				{
					scrollBar.Scroll(-mouse->Delta);
				}
				else if (mouse->State == MouseEvent::Move)
				{
					if (drag)
					{
						int itemIndex = scrollBar.GetPosition() + (mouse->Position.Y - itemsRect.GetTop()) / 12;

						if (itemIndex >= scrollBar.GetPosition() && itemIndex < items.GetSize() && itemIndex < scrollBar.GetPosition() + scrollBar.GetPageSize())
						{
							selectedIndex = itemIndex;
						}
						else if (itemIndex < scrollBar.GetPosition())
						{
							scrollBar.Scroll(-1);
							selectedIndex = scrollBar.GetPosition();
						}
						else if (itemIndex >= scrollBar.GetPosition() + scrollBar.GetPageSize())
						{
							scrollBar.Scroll(1);
							selectedIndex = scrollBar.GetPosition() + scrollBar.GetPageSize();
							if (selectedIndex > items.GetSize())
							{
								selectedIndex = items.GetSize();
							}
							--selectedIndex;
						}
					}
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					drag = false;

					if (selectedIndex != -1)
					{
						if (changeFunc != NULL)
						{
							(*changeFunc)(this);
						}
					}
				}
			}
			return Event::None;
		}
		else if (event->Type == Event::Keyboard)
		{
			if (items.IsEmpty())
			{
				return Event::None;
			}
		
			KeyboardEvent *keyboard = (KeyboardEvent*) event;
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
							selectedIndex = items.GetSize() - 1;
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
					if (selectedIndex >= items.GetSize())
					{
						selectedIndex = items.GetSize() - 1;
					}

					if (oldSelectedIndex != selectedIndex)
					{
						scrollBar.ShowItem(selectedIndex);
						
						if (changeFunc != NULL)
						{
							(*changeFunc)(this);
						}
					}
					return Event::None;
			}
		}
		else if (event->Type == Event::System)
		{
			SystemEvent *system = (SystemEvent*)event;
			if (system->State == SystemEvent::CaptureChanged)
			{
				drag = false;
			}
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void ListBox::Render(Drawing::IRenderer *renderer)
	{
		if (needRepaint)
		{
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture());
			}
			
			Drawing::Size size = bounds.GetSize();
			
			Drawing::ITexture *main = texture.Get(0);
			
			main->Create(size);
			main->BeginUpdate();
			main->Clear();
			
			main->FillGradient(1, 1, size.Width - 2, size.Height - 2, Drawing::Color(0xFF353432), Drawing::Color(0xFF171614));

			Drawing::Color border(0x804C4B47);
			
			main->Fill(0, 1, 1, size.Height - 2, border);
			main->Fill(size.Width - 1, 1, 1, size.Height - 2, border);
			main->Fill(1, 0, size.Width - 2, 1, border);
			main->Fill(1, size.Height - 1, size.Width - 2, 1, border);

			main->Fill(1, 1, 1, 1, border);
			main->Fill(size.Width - 2, 1, 1, 1, border);
			main->Fill(1, size.Height - 2, 1, 1, border);
			main->Fill(size.Width - 2, size.Height - 2, 1, 1, border);
			
			main->EndUpdate();
		}
		
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture.Get(0), bounds.GetPosition());
		
		renderer->SetRenderColor(foreColor);
		//renderer->RenderText(font, captionBar, text);
	
		scrollBar.Render(renderer);
	}
	//---------------------------------------------------------------------------
}