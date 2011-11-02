#include "ComboBox.hpp"
#include "..\Misc\Exceptions.hpp"

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
		SetSize(160, 24);
		
		SetBackColor(Drawing::Color(0xFF4E4E4E));
		SetDropDownColor(Drawing::Color(0xFF171614));
		SetForeColor(Drawing::Color(0xFFE5E0E4));
	}
	//---------------------------------------------------------------------------
	ComboBox::~ComboBox()
	{
		Clear();
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ComboBox::SetDropDownColor(const Drawing::Color &color)
	{
		dropDownColor = color;
	}
	//---------------------------------------------------------------------------
	Drawing::Color ComboBox::GetDropDownColor() const
	{
		return dropDownColor;
	}
	//---------------------------------------------------------------------------
	void ComboBox::SetFocus(bool focus)
	{
		Button::SetFocus(focus);
		if (!hasFocus)
		{
			open = false;
		}
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& ComboBox::GetItem(int index) const
	{
		if (index < 0 || index >= (int)items.size())
		{
			throw Misc::ArgumentOutOfRangeException(L"index", __WFILE__, __LINE__);
		}

		return items[index];
	}
	//---------------------------------------------------------------------------
	int ComboBox::GetSelectedIndex() const
	{
		return selectedIndex;
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& ComboBox::GetSelectedItem() const
	{
		return GetItem(GetSelectedIndex());
	}
	//---------------------------------------------------------------------------
	int ComboBox::GetItemsCount() const
	{
		return items.size();
	}
	//---------------------------------------------------------------------------
	SelectedIndexChangedEvent& ComboBox::GetSelectedIndexChangedEvent()
	{
		return selectedIndexChangedEvent;
	}
	//---------------------------------------------------------------------------
	KeyDownEvent& ComboBox::GetKeyDownEvent()
	{
		return keyDownEvent;
	}
	//---------------------------------------------------------------------------
	KeyPressEvent& ComboBox::GetKeyPressEvent()
	{
		return keyPressEvent;
	}
	//---------------------------------------------------------------------------
	KeyUpEvent& ComboBox::GetKeyUpEvent()
	{
		return keyUpEvent;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ComboBox::ContainsPoint(const Drawing::Point &point) const
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
	void ComboBox::AddItem(const Misc::UnicodeString &text)
	{
		InsertItem(items.size() > 0 ? items.size() : 0, text);
	}
	//---------------------------------------------------------------------------
	void ComboBox::InsertItem(int index, const Misc::UnicodeString &text)
	{
		items.insert(items.begin() + index, text);

		scrollBar.SetRange(items.size());

		if (selectedIndex == -1)
		{
			selectedIndex = 0;
		}
		
		if (GetItemsCount() == 1)
		{
			selectedIndex = 0;
			mouseOverItemIndex = -1;
			
			textHelper.SetText(GetSelectedItem());

			selectedIndexChangedEvent.Invoke(this);
		}

		Invalidate();
	}
	//---------------------------------------------------------------------------
	void ComboBox::RemoveItem(int index)
	{
		if (index < 0 || index >= (int)items.size())
		{
			throw Misc::ArgumentOutOfRangeException(L"index", __WFILE__, __LINE__);
		}
		
		items.erase(items.begin() + index);
		
		scrollBar.SetRange(items.size());
		
		if (selectedIndex >= (int)items.size())
		{
			selectedIndex = items.size() - 1;

			textHelper.SetText(GetSelectedItem());
			
			selectedIndexChangedEvent.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
	void ComboBox::Clear()
	{
		items.clear();
		
		scrollBar.SetRange(1);
		
		selectedIndex = -1;
		firstVisibleItemIndex = 0;
		mouseOverItemIndex = -1;
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
		if (selectedIndex >= (int)items.size())
		{
			selectedIndex = items.size() - 1;
		}

		if (oldSelectedIndex != selectedIndex)
		{
			textHelper.SetText(GetSelectedItem());

			selectedIndexChangedEvent.Invoke(this);
		}
	}
	//---------------------------------------------------------------------------
	void ComboBox::Invalidate()
	{
		Button::Invalidate();

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
		if (items.size() > 0)
		{
			scrollBar.ShowItem(selectedIndex);
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes ComboBox::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
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

					MouseEventArgs args(mouse);
					mouseDownEvent.Invoke(this, args);
				}
				else if (!open || !Drawing::Rectangle(0, clientArea.GetHeight() + 1, dropDownRect.GetWidth(), dropDownRect.GetHeight()).Contains(mouse->Position)) //dropDownRect
				{
					mouse->Position = mousePositionBackup;

					if (open)
					{
						open = false;
					}

					return IEvent::Continue;
				}
			}
			else if (mouse->State == MouseEvent::Scroll && open)
			{
				if (!hasFocus)
				{
					mouse->Position = mousePositionBackup;

					return IEvent::Continue;
				}
			}

			if (open)
			{
				mouse->Position.Y -= clientArea.GetHeight();
				if (scrollBar.ProcessEvent(event) == IEvent::DontContinue)
				{
					firstVisibleItemIndex = scrollBar.GetPosition();

					return IEvent::DontContinue;
				}
				mouse->Position.Y += clientArea.GetHeight();
			}
			
			if (mouse->State == MouseEvent::LeftDown)
			{
				if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position)) //clientArea
				{
					pressed = true;

					if (hasFocus)
					{
						open = !open;
					}

					MouseEventArgs args(mouse);
					mouseDownEvent.Invoke(this, args);

					return IEvent::DontContinue;
				}
			}
			else if (mouse->State == MouseEvent::Move)
			{
				if (open && Drawing::Rectangle(4, clientArea.GetHeight() + 4, dropDownRect.GetWidth() - (scrollBar.GetVisible() ? scrollBar.GetWidth() : 0) - 8, dropDownRect.GetHeight() - 8).Contains(mouse->Position)) //itemsRect
				{
					for (unsigned int i = 0; (int)i < itemsRect.GetHeight() / (font->GetSize() + 2) && i < items.size(); ++i)
					{
						if ((int)i * (font->GetSize() + 2) + (font->GetSize() + 2) > mouse->Position.Y - (itemsRect.GetTop() - bounds.GetTop()))
						{
							mouseOverItemIndex = i + firstVisibleItemIndex;
							break;
						}
					}

					MouseEventArgs args(mouse);
					mouseMoveEvent.Invoke(this, args);

					return IEvent::DontContinue;
				}
				else if (Drawing::Rectangle(0, 0, clientArea.GetWidth(), clientArea.GetHeight()).Contains(mouse->Position))
				{
					MouseEventArgs args(mouse);
					mouseMoveEvent.Invoke(this, args);

					return IEvent::DontContinue;
				}
			}
			else if (mouse->State == MouseEvent::LeftUp)
			{
				if (open && Drawing::Rectangle(4, clientArea.GetHeight() + 4, dropDownRect.GetWidth() - (scrollBar.GetVisible() ? scrollBar.GetWidth() : 0) - 8, dropDownRect.GetHeight() - 8).Contains(mouse->Position)) //itemsRect
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

						textHelper.SetText(GetSelectedItem());

						selectedIndexChangedEvent.Invoke(this);
					}

					open = false;

					clickEvent.Invoke(this);

					MouseEventArgs args(mouse);
					mouseClickEvent.Invoke(this, args);

					args = MouseEventArgs(mouse);
					mouseUpEvent.Invoke(this, args);

					return IEvent::DontContinue;
				}
			}
			
			return IEvent::Continue;
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
				KeyEventArgs args(keyboard);
				keyDownEvent.Invoke(this, args);
			
				if (open)
				{
					switch (keyboard->KeyCode)
					{
						case Key::Up:
						case Key::Down:
						case Key::Home:
						case Key::End:
						case Key::PageUp:
						case Key::PageDown:
							int oldMouseOverItemIndex = mouseOverItemIndex;

							switch (keyboard->KeyCode)
							{
								case Key::Up:
									--mouseOverItemIndex;
									break;
								case Key::Down:
									++mouseOverItemIndex;
									break;
								case Key::Home:
									mouseOverItemIndex = 0;
									break;
								case Key::End:
									mouseOverItemIndex = items.size() - 1;
									break;
								case Key::PageUp:
									mouseOverItemIndex += scrollBar.GetPageSize() - 1;
									break;
								case Key::PageDown:
									mouseOverItemIndex -= scrollBar.GetPageSize() - 1;
									break;
							}

							if (mouseOverItemIndex < 0)
							{
								mouseOverItemIndex = 0;
							}
							if (mouseOverItemIndex >= (int)items.size())
							{
								mouseOverItemIndex = items.size() - 1;
							}

							if (oldMouseOverItemIndex != mouseOverItemIndex)
							{
								if (scrollBar.ShowItem(mouseOverItemIndex))
								{
									firstVisibleItemIndex = scrollBar.GetPosition();
								}
							}
							break;
					}
				}
				else
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
								case Key::PageUp:
									selectedIndex = 0;
									break;
								case Key::End:
								case Key::PageDown:
									selectedIndex = items.size() - 1;
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
								textHelper.SetText(GetSelectedItem());
							
								selectedIndexChangedEvent.Invoke(this);
							}
							break;
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
						
						scrollBar.ShowItem(selectedIndex);
						
						textHelper.SetText(GetSelectedItem());
						
						selectedIndexChangedEvent.Invoke(this);
					}
				}
			}
			else if (keyboard->State == KeyboardEvent::KeyUp)
			{
				KeyEventArgs args(keyboard);
				keyUpEvent.Invoke(this, args);
			
				switch (keyboard->KeyCode)
				{
					case Key::Return:
						if (open)
						{
							if (selectedIndex != mouseOverItemIndex)
							{
								selectedIndex = mouseOverItemIndex;

								textHelper.SetText(GetSelectedItem());
								
								selectedIndexChangedEvent.Invoke(this);
							}
							open = false;
						}
						break;
				}
			}
			
			return IEvent::DontContinue;
		}
		
		return IEvent::Continue;
	}
	//---------------------------------------------------------------------------
	void ComboBox::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
		
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
			renderer->SetRenderColor(dropDownColor);
			renderer->Fill(dropDownRect.GetLeft() + 1, dropDownRect.GetTop() + 1, dropDownRect.GetWidth() - 2, dropDownRect.GetHeight() - 2);
			renderer->FillGradient(dropDownRect.GetLeft() + 2, dropDownRect.GetTop() + 2, dropDownRect.GetWidth() - 4, dropDownRect.GetHeight() - 4, backColor - Drawing::Color(0, 20, 20, 20));

			renderer->SetRenderColor(foreColor);
			for (unsigned int i = 0; (int)i < itemsRect.GetHeight() / (font->GetSize() + 2) && i + firstVisibleItemIndex < items.size(); ++i)
			{
				if ((int)i + firstVisibleItemIndex == mouseOverItemIndex)
				{
					renderer->SetRenderColor(Drawing::Color::Black());
					renderer->Fill(itemsRect.GetLeft() - 1, itemsRect.GetTop() + i * (font->GetSize() + 2) - 1, itemsRect.GetWidth() + 2, font->GetSize() + 4);
					renderer->SetRenderColor(foreColor);
				}
				
				renderer->RenderText(font, itemsRect.GetLeft(), itemsRect.GetTop() + i * (font->GetSize() + 2), itemsRect.GetWidth(), font->GetSize() + 2, items[firstVisibleItemIndex + i]);
			}

			scrollBar.Render(renderer);
		}

		if (controls.size() > 0)
		{
			Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
			renderer->SetRenderRectangle(clientArea + renderRect.GetPosition());
			
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				controls[i]->Render(renderer);
			}
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}