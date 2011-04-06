#include "ComboBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ComboBox::ComboBox(Panel *parentPanel) : scrollBar(parentPanel)
	{
		type = CONTROL_COMBOBOX;
		
		ParentPanel = parentPanel;
		
		dropdownHeight = COMBOBOX_MAX_HEIGHT;
		
		selectedIndex = -1;
		focusedIndex = -1;
		opened = false;
	}
	//---------------------------------------------------------------------------
	ComboBox::~ComboBox()
	{
		Clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	ListItem* ComboBox::GetItem(int index)
	{
		if (index < 0 || index >= items.GetSize())
		{
			return NULL;
		}

		return items.Get(index);
	}
	//---------------------------------------------------------------------------
	int ComboBox::GetSelectedIndex()
	{
		return selectedIndex;
	}
	//---------------------------------------------------------------------------
	ListItem* ComboBox::GetSelectedItem()
	{
		return GetItem(GetSelectedIndex());
	}
	//---------------------------------------------------------------------------
	int ComboBox::GetItemsCount()
	{
		return items.GetSize();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ComboBox::CanHaveFocus()
	{
		return visible && enabled;
	}
	//---------------------------------------------------------------------------
	bool ComboBox::ContainsPoint(const Drawing::Point &point)
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void ComboBox::UpdateRects()
	{
		Button::UpdateRects();

		buttonRect = bounds;
		buttonRect.SetLeft(buttonRect.GetRight() - buttonRect.GetHeight());
		
		textRect = bounds;
		textRect.Inflate(-buttonRect.GetLeft(), 0); //todo

		dropdownRect = textRect;
		dropdownRect.Offset(0, (int)(0.9f * textRect.GetHeight()));
		dropdownRect.SetHeight(dropdownHeight);
		dropdownRect.Inflate(-scrollBar.GetBounds().GetWidth(), 0);

		itemsRect = dropdownRect;
		int offsetH = (int)(0.1f * dropdownRect.GetWidth()),
			offsetV = (int)(0.1f * dropdownRect.GetHeight());
		itemsRect.Offset(offsetH, offsetV);
		itemsRect.Inflate(-offsetH, -offsetV);
		
		scrollBar.SetLocation(Drawing::Point(dropdownRect.GetRight(), dropdownRect.GetTop()));
		scrollBar.SetSize(Drawing::Size(-1, dropdownRect.GetHeight()));
		scrollBar.SetPageSize(itemsRect.GetHeight() / COMBOBOX_ITEM_HEIGHT);
		scrollBar.ShowItem(selectedIndex);
	}
	//---------------------------------------------------------------------------
	bool ComboBox::AddItem(const char *itemText)
	{
		if (itemText == NULL)
		{
			return false;
		}
	
		ListItem *newItem = new ListItem();
		if (newItem == NULL)
		{
			return false;
		}
		
		memset((void*)newItem, 0x00, sizeof(ListItem));
		
		strcpy_s(newItem->Text, 256, itemText);
			
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
			
			if (GetItemsCount() == 1)
			{
				selectedIndex = 0;
				focusedIndex = 0;
				
				if (changeFunc != NULL)
				{
					(*changeFunc)(this);
				}
			}
			
			return true;
		}
	}
	//---------------------------------------------------------------------------
	bool ComboBox::InsertItem(int index, const char *itemText)
	{
		if (itemText == NULL)
		{
			return false;
		}
	
		ListItem *newItem = new ListItem();
		if (newItem == NULL)
		{
			return false;
		}
		
		memset((void*)newItem, 0x00, sizeof(ListItem));

		strcpy_s(newItem->Text, 256, itemText);
			
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
			
			if (GetItemsCount() == 1)
			{
				selectedIndex = 0;
				focusedIndex = 0;
				
				if (changeFunc != NULL)
				{
					(*changeFunc)(this);
				}
			}
			return true;
		}
	}
	//---------------------------------------------------------------------------
	bool ComboBox::RemoveItem(int index)
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
	bool ComboBox::Clear()
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
		focusedIndex = -1;

		return true;
	}
	//---------------------------------------------------------------------------
	bool ComboBox::Contains(const char* text)
	{
		return Find(text) != -1;
	}
	//---------------------------------------------------------------------------
	int ComboBox::Find(const char* text)
	{
		if (text == NULL)
		{
			return -1;
		}

		for (int i = 0, len = items.GetSize(); i < len; i++)
		{
			ListItem *item = items.Get(i);

			if (strcmp(item->Text, text) == 0)
			{
				return i;
			}
		}

		return -1;
	}
	//---------------------------------------------------------------------------
	void ComboBox::SelectItem(int newIndex)
	{
		if (items.GetSize() == 0)
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
			if (changeFunc != NULL)
			{
				(*changeFunc)(this);
			}
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes ComboBox::ProcessEvent(Event *event)
	{
		if (event == NULL || !visible || !enabled)
		{
			return Event::None;
		}
		
		if (scrollBar.ProcessEvent(event) == Event::None)
		{
			return Event::None;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*) event;
			
			if (!items.IsEmpty())
			{
				if (mouse->State == MouseEvent::LeftDown)
				{
					if (bounds.Contains(mouse->Position))
					{
						pressed = true;
						//SetCapture(this);

						if (!hasFocus)
						{
							ParentPanel->RequestFocus(this);
						}

						if (hasFocus)
						{
							opened = !opened;
						
							if (!opened)
							{
								ParentPanel->ClearFocus();
							}
						}

						return Event::None;
					}

					if (opened && itemsRect.Contains(mouse->Position))
					{
						for (int i = scrollBar.GetPosition(), len = items.GetSize(); i < len; i++)
						{
							ListItem *item = items.Get(i);
							if (item->ItemRect.Contains(mouse->Position))
							{
								focusedIndex = selectedIndex = i;
								
								if (changeFunc != NULL)
								{
									(*changeFunc)(this);
								}
								
								opened = false;
								
								ParentPanel->ClearFocus();

								break;
							}
						}

						return Event::None;
					}

					if (opened)
					{
						focusedIndex = selectedIndex;

						if (changeFunc != NULL)
						{
							(*changeFunc)(this);
						}
						
						opened = false;
					}

					pressed = false;

					ParentPanel->ClearFocus();
				}
				else if (mouse->State == MouseEvent::Wheel)
				{
					if (opened)
					{
						scrollBar.Scroll(-mouse->Delta);
					}
					else
					{
						if (mouse->Delta > 0 )
						{
							if (focusedIndex > 0)
							{
								focusedIndex--;
								selectedIndex = focusedIndex;     
								
								if (changeFunc != NULL)
								{
									(*changeFunc)(this);
								}
							}          
						}
						else
						{
							if (focusedIndex + 1 < GetItemsCount())
							{
								focusedIndex++;
								selectedIndex = focusedIndex;   

								if (changeFunc != NULL)
								{
									(*changeFunc)(this);
								}
							}
						}
					}
				}
				else if (mouse->State == MouseEvent::Move)
				{
					if (opened && itemsRect.Contains(mouse->Position))
					{
						for (int i = 0, len = items.GetSize(); i < len; i++)
						{
							ListItem *item = items[i];
							if (item->ItemRect.Contains(mouse->Position))
							{
								focusedIndex = i;
								break;
							}
						}
						return Event::None;
					}
				}
				else if (mouse->State == MouseEvent::LeftUp)
				{
					if (pressed && bounds.Contains(mouse->Position))
					{
						pressed = false;
						//ReleaseCapture();
						return Event::None;
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
				case Key::Return:
					if (opened)
                    {
                        if (selectedIndex != focusedIndex)
                        {
                            selectedIndex = focusedIndex;
                            
							if (changeFunc != NULL)
							{
								(*changeFunc)(this);
							}
                        }
                        opened = false;
                        
                        return Event::None;
                    }
                    break;
				case Key::Down:
				case Key::Left:
                    if (focusedIndex > 0)
                    {
                        focusedIndex--;
                        selectedIndex = focusedIndex;

                        if (!opened)
						{
                            if (changeFunc != NULL)
							{
								(*changeFunc)(this);
							}
						}
                    }
                    return Event::None;
                case Key::Right:
                case Key::Up:
                    if (focusedIndex + 1 < GetItemsCount())
                    {
                        focusedIndex++;
                        selectedIndex = focusedIndex;

                        if (!opened)
						{
                            if (changeFunc != NULL)
							{
								(*changeFunc)(this);
							}
						}
                    }
					return Event::None;
			}
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void ComboBox::Render(Drawing::IRenderer *renderer)
	{
		//OK
		/*button:
		Drawing::Size size(220, 24);

		texture = new Drawing::Texture(pDevice);
		texture->Create(size);
		texture->BeginUpdate();
		texture->Clear();

		texture->FillGradient(1, 1, size.Width - 2, size.Height - 2, 0xFF635F5B, 0xFF4E4D4A);

		texture->Fill(1, 0, size.Width - 2, 1, 0x60FFFFFF);
		texture->Fill(0, 1, 1, size.Height - 2, 0x60FFFFFF);
		texture->Fill(1, size.Height - 1, size.Width - 2, 1, 0x60FFFFFF);
		texture->Fill(size.Width - 1, 1, 1, size.Height - 2, 0x60FFFFFF);

		texture->Fill(1, 1, 1, 1, 0x60FFFFFF);
		texture->Fill(size.Width - 2, 1, 1, 1, 0x60FFFFFF);
		texture->Fill(1, size.Height - 2, 1, 1, 0x60FFFFFF);
		texture->Fill(size.Width - 2, size.Height - 2, 1, 1, 0x60FFFFFF);

		texture->Fill(size.Width - 20, 4, 1, 16, 0x60FFFFFF);

		texture->Fill(size.Width - 11, 14, 1, 1, 0xFFBAB8B9);
		texture->Fill(size.Width - 12, 13, 3, 1, 0xFFBAB8B9);
		texture->Fill(size.Width - 13, 12, 5, 1, 0xFFBAB8B9);
		texture->Fill(size.Width - 14, 11, 7, 1, 0xFFBAB8B9);

		texture2->EndUpdate();*/

		/*dropdown:
		Drawing::Size size(220, 224);

		texture2 = new Drawing::Texture(pDevice);
		texture2->Create(size);
		texture2->BeginUpdate();
		texture2->Clear();

		texture->FillGradient(1, 1, size.Width - 2, size.Height - 2, 0xFF625E5A, 0xFF433F3E);

		texture->Fill(0, 1, 1, size.Height - 2, 0x8059595A);
		texture->Fill(size.Width - 1, 1, 1, size.Height - 2, 0x8059595A);
		texture->Fill(1, 0, size.Width - 2, 1, 0x8059595A);
		texture->Fill(1, size.Height - 1, size.Width - 2, 1, 0x8059595A);

		texture->Fill(1, 1, 1, 1, 0x8059595A);
		texture->Fill(size.Width - 2, 1, 1, 1, 0x8059595A);
		texture->Fill(1, size.Height - 2, 1, 1, 0x8059595A);
		texture->Fill(size.Width - 2, size.Height - 2, 1, 1, 0x8059595A);

		texture2->EndUpdate();*/
		
		renderer->SetRenderColor(backColor);
		renderer->RenderTexture(texture, bounds.GetPosition());

		scrollBar.Render(renderer);
	}
	//---------------------------------------------------------------------------
	void ComboBox::OnFocusOut()
	{
		opened = false;
	}
	//---------------------------------------------------------------------------
}