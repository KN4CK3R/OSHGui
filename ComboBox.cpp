#include "ComboBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ComboBox::ComboBox(Control *parent) : Button(parent), scrollBar(this)
	{
		type = CONTROL_COMBOBOX;
				
		dropdownHeight = COMBOBOX_MAX_HEIGHT;
		
		selectedIndex = -1;
		mouseOverItemIndex = -1;
		firstVisibleItemIndex = 0;
		open = false;

		SetAutoSize(false);
		SetSize(Drawing::Size(160, 24));
		
		SetBackColor(Drawing::Color(0xFF4E4E4E));
		SetForeColor(Drawing::Color::White());
	}
	//---------------------------------------------------------------------------
	ComboBox::~ComboBox()
	{
		Clear();

		Button::~Button();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	ListItem* ComboBox::GetItem(int index)
	{
		if (index < 0 || index >= items.size())
		{
			return NULL;
		}

		return items.at(index);
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
		return items.size();
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
		if (bounds.Contains(point))
		{
			return true;
		}
		if (open && dropDownRect.Contains(point))
		{
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------
	/*void ComboBox::OnFocusOut()
	{
		open = false;
	}*/
	//---------------------------------------------------------------------------
	bool ComboBox::AddItem(const Misc::UnicodeString &text)
	{	
		return InsertItem(items.size() > 0 ? items.size() : 0, text);
	}
	//---------------------------------------------------------------------------
	bool ComboBox::InsertItem(int index, const Misc::UnicodeString &text)
	{	
		ListItem *newItem = new ListItem();
		if (newItem == NULL)
		{
			return false;
		}
		
		newItem->Text = text;
		
		items.insert(items.begin() + index, newItem);

		scrollBar.SetRange(items.size());

		Invalidate();
			
		if (GetItemsCount() == 1)
		{
			selectedIndex = 0;
			mouseOverItemIndex = -1;
			
			textHelper.SetText(GetSelectedItem()->Text);

			changeEventHandler.Invoke(this);
		}

		return true;
	}
	//---------------------------------------------------------------------------
	bool ComboBox::RemoveItem(int index)
	{
		if (index < 0 || index >= items.size())
		{
			return false;
		}

		ListItem *item = items.at(index);
		delete item;
		item = NULL;
		
		items.erase(items.begin() + index);
		
		scrollBar.SetRange(items.size());
		
		if (selectedIndex >= items.size())
		{
			selectedIndex = items.size() - 1;

			textHelper.SetText(GetSelectedItem()->Text);
			
			changeEventHandler.Invoke(this);
		}

		return true;
	}
	//---------------------------------------------------------------------------
	bool ComboBox::Clear()
	{
		for (int i = 0; i < items.size(); i++)
		{
			ListItem *item = items.at(i);
			delete item;
			item = NULL;
		}

		items.clear();
		
		scrollBar.SetRange(1);
		
		selectedIndex = -1;
		firstVisibleItemIndex = 0;
		mouseOverItemIndex = -1;

		return true;
	}
	//---------------------------------------------------------------------------
	void ComboBox::SelectItem(int newIndex)
	{
		if (items.size() == 0)
		{
			return;
		}

		int oldSelectedIndex = selectedIndex;

		selectedIndex = newIndex;

		if (selectedIndex < 0)
		{
			selectedIndex = 0;
		}
		if (selectedIndex >= items.size())
		{
			selectedIndex = items.size() - 1;
		}

		if (oldSelectedIndex != selectedIndex)
		{
			textHelper.SetText(GetSelectedItem()->Text);

			changeEventHandler.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
	void ComboBox::Invalidate()
	{
		Button::Invalidate();

		buttonRect = bounds;
		buttonRect.SetLeft(buttonRect.GetRight() - buttonRect.GetHeight());
		
		textRect = bounds;
		textRect.Inflate(-buttonRect.GetLeft(), 0); //todo

		dropDownRect = Drawing::Rectangle(bounds.GetLeft(), bounds.GetBottom() + 1, bounds.GetWidth(), 0);
		if (items.size() == 0)
		{
			dropDownRect.SetHeight(font->GetSize() + 4);
		}
		else
		{
			int height = items.size() * (font->GetSize() + 4);
			if (height > COMBOBOX_MAX_HEIGHT)
			{
				height = COMBOBOX_MAX_HEIGHT;
			}
			dropDownRect.SetHeight(height);
		}

		Drawing::Rectangle boundsBackup = bounds;
		bounds = dropDownRect;

		scrollBar.Invalidate();

		bounds = boundsBackup;

		itemsRect = Drawing::Rectangle(dropDownRect.GetLeft() + 4, dropDownRect.GetTop() + 4, dropDownRect.GetWidth() - (scrollBar.GetVisible() ? scrollBar.GetWidth() : 0) - 8, dropDownRect.GetHeight() - 8);
		
		scrollBar.SetPageSize(itemsRect.GetHeight() / (font->GetSize() + 2));
		scrollBar.ShowItem(selectedIndex);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	Event::NextEventTypes ComboBox::ProcessEvent(Event *event)
	{
		if (event == NULL)
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
			else if (mouse->State == MouseEvent::Scroll && open)
			{
				if (!hasFocus)
				{
					mouse->Position = mousePositionBackup;

					return Event::Continue;
				}
			}
		}
		
		if (open && scrollBar.ProcessEvent(event) == Event::DontContinue)
		{
			firstVisibleItemIndex = scrollBar.GetPosition();

			return Event::DontContinue;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			
			if (mouse->State == MouseEvent::LeftDown)
			{
				if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position)) //clientArea
				{
					pressed = true;

					if (hasFocus)
					{
						open = !open;
					}

					return Event::DontContinue;
				}
			}
			/*else if (mouse->State == MouseEvent::Scroll)
			{
				if (open)
				{
					scrollBar.Scroll(-mouse->Delta);
				}
				else
				{
					if (mouse->Delta > 0 )
					{
						if (mouseOverItemIndex > 0)
						{
							mouseOverItemIndex--;
							selectedIndex = mouseOverItemIndex;
								
							changeEventHandler.Invoke(this);
						}
					}
					else
					{
						if (mouseOverItemIndex + 1 < GetItemsCount())
						{
							mouseOverItemIndex++;
							selectedIndex = mouseOverItemIndex;

							changeEventHandler.Invoke(this);
						}
					}
				}
			}*/
			else if (mouse->State == MouseEvent::Move)
			{
				if (open && Drawing::Rectangle(4, clientArea.GetHeight() + 4, dropDownRect.GetWidth() - (scrollBar.GetVisible() ? scrollBar.GetWidth() : 0) - 8, dropDownRect.GetHeight() - 8).Contains(mouse->Position)) //itemsRect
				{
					for (unsigned int i = 0; i < itemsRect.GetHeight() / (font->GetSize() + 2) && i < items.size(); ++i)
					{
						if (i * (font->GetSize() + 2) + (font->GetSize() + 2) > mouse->Position.Y - (itemsRect.GetTop() - bounds.GetTop()))
						{
							mouseOverItemIndex = i;
							break;
						}
					}

					return Event::DontContinue;
				}
			}
			else if (mouse->State == MouseEvent::LeftUp)
			{
				if (open && Drawing::Rectangle(4, clientArea.GetHeight() + 4, dropDownRect.GetWidth() - (scrollBar.GetVisible() ? scrollBar.GetWidth() : 0) - 8, dropDownRect.GetHeight() - 8).Contains(mouse->Position)) //itemsRect
				{
					int itemIndex = -1;
					for (unsigned int i = 0; i < itemsRect.GetHeight() / (font->GetSize() + 2) && i < items.size(); ++i)
					{
						if (i * (font->GetSize() + 2) + (font->GetSize() + 2) > mouse->Position.Y - (itemsRect.GetTop() - bounds.GetTop()))
						{
							itemIndex = i;
							break;
						}
					}

					if (itemIndex != -1)
					{
						selectedIndex = itemIndex + firstVisibleItemIndex;

						textHelper.SetText(GetSelectedItem()->Text);

						changeEventHandler.Invoke(this);
					}

					open = false;

					return Event::DontContinue;
				}
			}
			
			return Event::DontContinue;
		}
		else if (event->Type == Event::Keyboard)
		{
			if (items.size() == 0)
			{
				return Event::DontContinue;
			}
		
			KeyboardEvent *keyboard = (KeyboardEvent*)event;
			switch (keyboard->KeyCode)
			{
				case Key::Return:
					if (open)
                    {
                        if (selectedIndex != mouseOverItemIndex)
                        {
                            selectedIndex = mouseOverItemIndex;
                            
							changeEventHandler.Invoke(this);
                        }
                        open = false;
                        
                        return Event::DontContinue;
                    }
                    break;
				case Key::Down:
				case Key::Left:
                    if (mouseOverItemIndex > 0)
                    {
                        mouseOverItemIndex--;
                        selectedIndex = mouseOverItemIndex;

                        if (!open)
						{
							changeEventHandler.Invoke(this);
						}
                    }
                    return Event::DontContinue;
                case Key::Right:
                case Key::Up:
                    if (mouseOverItemIndex + 1 < GetItemsCount())
                    {
                        mouseOverItemIndex++;
                        selectedIndex = mouseOverItemIndex;

                        if (!open)
						{
							changeEventHandler.Invoke(this);
						}
                    }
					return Event::DontContinue;
			}
		}
		
		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void ComboBox::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
	
		/*//OK
		if (needRepaint)
		{
			if (texture.IsEmpty())
			{
				texture.Add(renderer->CreateNewTexture()); //Button
				texture.Add(renderer->CreateNewTexture()); //DropDown
			}
			
			Drawing::Size size = bounds.GetSize();
			Drawing::ITexture *button = texture.Get(0);
			{
				button->Create(size);
				button->BeginUpdate();
				button->Clear();

				button->FillGradient(1, 1, size.Width - 2, size.Height - 2, Drawing::Color(0xFF635F5B), Drawing::Color(0xFF4E4D4A));

				Drawing::Color border(0x60FFFFFF);
				
				button->Fill(1, 0, size.Width - 2, 1, border);
				button->Fill(0, 1, 1, size.Height - 2, border);
				button->Fill(1, size.Height - 1, size.Width - 2, 1, border);
				button->Fill(size.Width - 1, 1, 1, size.Height - 2, border);

				button->Fill(1, 1, 1, 1, border);
				button->Fill(size.Width - 2, 1, 1, 1, border);
				button->Fill(1, size.Height - 2, 1, 1, border);
				button->Fill(size.Width - 2, size.Height - 2, 1, 1, border);

				button->Fill(size.Width - 20, 4, 1, 16, border);
				
				border = Drawing::Color(0xFFBAB8B9);

				button->Fill(size.Width - 11, 14, 1, 1, border);
				button->Fill(size.Width - 12, 13, 3, 1, border);
				button->Fill(size.Width - 13, 12, 5, 1, border);
				button->Fill(size.Width - 14, 11, 7, 1, border);

				button->EndUpdate();
			}

			size = dropDownRect.GetSize();
			Drawing::ITexture *dropdown = texture.Get(1);
			{
				dropdown->Create(size);
				dropdown->BeginUpdate();
				dropdown->Clear();
				
				dropdown->FillGradient(1, 1, size.Width - 2, size.Height - 2, Drawing::Color(0xFF625E5A), Drawing::Color(0xFF433F3E));

				Drawing::Color border(0x8059595A);
				
				dropdown->Fill(0, 1, 1, size.Height - 2, border);
				dropdown->Fill(size.Width - 1, 1, 1, size.Height - 2, border);
				dropdown->Fill(1, 0, size.Width - 2, 1, border);
				dropdown->Fill(1, size.Height - 1, size.Width - 2, 1, border);

				dropdown->Fill(1, 1, 1, 1, border);
				dropdown->Fill(size.Width - 2, 1, 1, 1, border);
				dropdown->Fill(1, size.Height - 2, 1, 1, border);
				dropdown->Fill(size.Width - 2, size.Height - 2, 1, 1, border);

				dropdown->EndUpdate();
			}
		}*/
		
		Drawing::Color tempColor = backColor;

		if (hasFocus || mouseOver)
		{
			tempColor += mouseOverFocusColor;
		}
		
		renderer->SetRenderColor(tempColor + Drawing::Color(0, 10, 10, 10));
		renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop(), bounds.GetWidth() - 2, bounds.GetHeight() - 1);
		renderer->Fill(bounds.GetLeft(), bounds.GetTop() + 1, bounds.GetWidth(), bounds.GetHeight() - 3);
		renderer->SetRenderColor(tempColor - Drawing::Color(0, 50, 50, 50));
		renderer->Fill(bounds.GetLeft() + 1, bounds.GetTop() + bounds.GetHeight() - 2, bounds.GetWidth() - 2, 2);
		renderer->Fill(bounds.GetLeft() + bounds.GetWidth() - 1, bounds.GetTop() + 1, 1, bounds.GetHeight() - 2);

		renderer->SetRenderColor(tempColor);
		renderer->FillGradient(bounds.GetLeft() + 1, bounds.GetTop() + 2, bounds.GetWidth() - 2, bounds.GetHeight() - 4, backColor - Drawing::Color(0, 20, 20, 20));
		renderer->FillGradient(bounds.GetLeft() + 2, bounds.GetTop() + 1, bounds.GetWidth() - 4, bounds.GetHeight() - 2, backColor - Drawing::Color(0, 20, 20, 20));

		renderer->SetRenderColor(foreColor);
		renderer->RenderText(font, bounds.GetLeft() + 6, bounds.GetTop() + 5, bounds.GetWidth() - 20, bounds.GetHeight() - 10, textHelper.GetText());

		for (int i = 0; i < 4; ++i)
		{
			renderer->Fill(bounds.GetRight() - 8 - i, bounds.GetBottom() - 11 - i, 1 + i * 2, 1);
		}
		
		if (open)
		{
			renderer->SetRenderColor(Drawing::Color::Black());
			renderer->Fill(dropDownRect);
			renderer->SetRenderColor(backColor);
			renderer->Fill(dropDownRect.GetLeft() + 1, dropDownRect.GetTop() + 1, dropDownRect.GetWidth() - 2, dropDownRect.GetHeight() - 2);
			renderer->FillGradient(dropDownRect.GetLeft() + 2, dropDownRect.GetTop() + 2, dropDownRect.GetWidth() - 4, dropDownRect.GetHeight() - 4, backColor - Drawing::Color(0, 20, 20, 20));

			renderer->SetRenderColor(foreColor);
			for (unsigned int i = 0; i < itemsRect.GetHeight() / (font->GetSize() + 2) && i + firstVisibleItemIndex < items.size(); ++i)
			{
				if (i == mouseOverItemIndex)
				{
					renderer->SetRenderColor(Drawing::Color::Orange());
					renderer->Fill(itemsRect.GetLeft(), itemsRect.GetTop() + i * (font->GetSize() + 2), itemsRect.GetWidth(), font->GetSize() + 2);
					renderer->SetRenderColor(foreColor);
				}

				renderer->RenderText(font, itemsRect.GetLeft(), itemsRect.GetTop() + i * (font->GetSize() + 2), itemsRect.GetWidth(), font->GetSize() + 2, items[firstVisibleItemIndex + i]->Text);
			}

			scrollBar.Render(renderer);
		}

		if (controls.size() > 0)
		{
			Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
			renderer->SetRenderRectangle(clientArea + renderRect.GetPosition());
			
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				controls.at(i)->Render(renderer);
			}
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}