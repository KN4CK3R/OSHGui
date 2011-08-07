#include "ListBox.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ListBox::ListBox(Control *parent) : Control(parent), scrollBar(this)
	{
		type = CONTROL_LISTBOX;
				
		selectedIndex = -1;
		drag = false;

		SetBackColor(Drawing::Color(0xFF121212));
		SetForeColor(Drawing::Color(0xFFB8B4B0));
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
		if (index < 0 || index >= items.size())
		{
			return NULL;
		}

		return items.at(index);
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
		return items.size();
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
	void ListBox::Invalidate()
	{
		clientArea = Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight());
		
		//ScrollBar can handle the bounds...
		scrollBar.SetBounds(0, 0, 0, 0);

		itemsRect = Drawing::Rectangle(2, 2, scrollBar.GetWidth() - 4, bounds.GetHeight() - 4);

		scrollBar.SetPageSize(itemsRect.GetHeight() / LISTBOX_ITEM_HEIGHT);
		scrollBar.ShowItem(selectedIndex);
	}
	//---------------------------------------------------------------------------
	bool ListBox::AddItem(const Misc::UnicodeString &text)
	{	
		return InsertItem(items.size() > 0 ? items.size() : 0, text);
	}
	//---------------------------------------------------------------------------
	bool ListBox::InsertItem(int index, const Misc::UnicodeString &text)
	{	
		ListItem *newItem = new ListItem();
		if (newItem == NULL)
		{
			return false;
		}
		
		newItem->Text = text;
		
		newItem->ItemRect = Drawing::Rectangle(0, 0, 0, 0);

		items.insert(items.begin() + index, newItem);

		scrollBar.SetRange(0, items.size());
		return true;
	}
	//---------------------------------------------------------------------------
	bool ListBox::RemoveItem(int index)
	{
		if (index < 0 || index >= items.size())
		{
			return false;
		}

		ListItem *item = items.at(index);
		delete item;
		item = NULL;
		
		items.erase(items.begin() + index);

		scrollBar.SetRange(0, items.size());
		if (selectedIndex >= items.size())
		{
			selectedIndex = items.size() - 1;
			
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
		for (unsigned int i = 0; i < items.size(); i++)
		{
			ListItem *item = items.at(i);
			delete item;
			item = NULL;
		}

		items.clear();
		
		scrollBar.SetRange(0, 1);
		
		selectedIndex = -1;

		return true;
	}
	//---------------------------------------------------------------------------
	void ListBox::SelectItem(int newIndex)
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
			return Event::DontContinue;
		}

		if (!hasFocus && event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			if (mouse->State == MouseEvent::LeftDown)
			{
				Drawing::Point mousePositionBackup = mouse->Position;
				mouse->Position = PointToClient(mouse->Position);

				if (clientArea.Contains(mouse->Position))
				{
					Parent->RequestFocus(this);
				}
			}
		}
				
		if (scrollBar.ProcessEvent(event) == Event::DontContinue)
		{
			return Event::DontContinue;
		}
	
		if (event->Type == Event::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			
			if (items.size() != 0 && scrollBarRect.Contains(mouse->Position))
			{
				if (mouse->State == MouseEvent::LeftDown)
				{
					int itemIndex = scrollBar.GetPosition() + (mouse->Position.Y - itemsRect.GetTop()) / 12;

					if (itemIndex >= scrollBar.GetPosition() && itemIndex < items.size() && itemIndex < scrollBar.GetPosition() + scrollBar.GetPageSize())
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

						if (itemIndex >= scrollBar.GetPosition() && itemIndex < items.size() && itemIndex < scrollBar.GetPosition() + scrollBar.GetPageSize())
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
							if (selectedIndex > items.size())
							{
								selectedIndex = items.size();
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
			return Event::DontContinue;
		}
		else if (event->Type == Event::Keyboard)
		{
			if (items.size() == 0)
			{
				return Event::DontContinue;
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
					if (selectedIndex >= items.size())
					{
						selectedIndex = items.size() - 1;
					}

					if (oldSelectedIndex != selectedIndex)
					{
						scrollBar.ShowItem(selectedIndex);
						
						if (changeFunc != NULL)
						{
							(*changeFunc)(this);
						}
					}
					return Event::DontContinue;
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
		if (!visible)
		{
			return;
		}
	
		/*if (needRepaint)
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
		}*/

		Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
		renderer->SetRenderRectangle(clientArea + bounds.GetPosition() + renderRect.GetPosition());
		
		renderer->SetRenderColor(backColor);
		renderer->Fill(1, 0, clientArea.GetWidth() - 2, clientArea.GetHeight());
		renderer->Fill(0, 1, clientArea.GetWidth(), clientArea.GetHeight() - 2);
		
		renderer->SetRenderColor(foreColor);
		for (unsigned int i = 0; i < items.size(); ++i)
		{
			renderer->RenderText(font, 2, 2 + i * (font->GetSize() + 2), clientArea.GetWidth() - 4, font->GetSize() + 2, items[i]->Text);
		}
	
		scrollBar.Render(renderer);

		renderer->SetRenderRectangle(renderRect);
	}
	//---------------------------------------------------------------------------
}