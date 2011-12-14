#include "ComboBox.hpp"
#include "ListBox.hpp"
#include "ScrollBar.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ComboBox::ComboBox()
	{
		type = CONTROL_COMBOBOX;
		
		droppedDown = false;

		button = new ComboBoxButton();
		button->SetLocation(0, 0);
		button->GetClickEvent() += ClickEventHandler([this](Control*)
		{
			if (!droppedDown)
			{
				Expand();
			}
			else
			{
				Collapse();
			}
		});
		AddSubControl(button);

		listBox = new ListBox();
		listBox->SetLocation(0, button->GetBottom() + 2);
		listBox->SetVisible(false);
		listBox->GetSelectedIndexChangedEvent() += SelectedIndexChangedEventHandler([this](Control*)
		{
			Collapse();
			button->SetText(listBox->GetSelectedItem());
		});
		AddSubControl(listBox);

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
	void ComboBox::SetSize(const Drawing::Size &size)
	{
		ContainerControl::SetSize(size);

		button->SetSize(size);
		listBox->SetLocation(0, button->GetBottom() + 2);
		listBox->SetSize(size.Width, listBox->GetHeight());
	}
	//---------------------------------------------------------------------------
	void ComboBox::SetFont(const std::shared_ptr<Drawing::IFont> &font)
	{
		ContainerControl::SetFont(font);

		button->SetFont(font);
		listBox->SetLocation(0, button->GetBottom() + 2);
		listBox->SetFont(font);
	}
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
	const Misc::AnsiString& ComboBox::GetItem(int index) const
	{
		return listBox->GetItem(index);
	}
	//---------------------------------------------------------------------------
	void ComboBox::SetSelectedIndex(int index)
	{
		listBox->SetSelectedIndex(index);

		OnLostFocus();
	}
	//---------------------------------------------------------------------------
	int ComboBox::GetSelectedIndex() const
	{
		return listBox->GetSelectedIndex();
	}
	//---------------------------------------------------------------------------
	void ComboBox::SetSelectedItem(const Misc::AnsiString &item)
	{
		listBox->SetSelectedItem(item);

		OnLostFocus();
	}
	//---------------------------------------------------------------------------
	const Misc::AnsiString& ComboBox::GetSelectedItem() const
	{
		return listBox->GetSelectedItem();
	}
	//---------------------------------------------------------------------------
	int ComboBox::GetItemsCount() const
	{
		return listBox->GetItemsCount();
	}
	//---------------------------------------------------------------------------
	SelectedIndexChangedEvent& ComboBox::GetSelectedIndexChangedEvent()
	{
		return listBox->GetSelectedIndexChangedEvent();
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void ComboBox::Expand()
	{
		droppedDown = true;
		listBox->SetVisible(true);
	}
	//---------------------------------------------------------------------------
	void ComboBox::Collapse()
	{
		droppedDown = false;
		listBox->SetVisible(false);
	}
	//---------------------------------------------------------------------------
	void ComboBox::AddItem(const Misc::AnsiString &text)
	{
		listBox->AddItem(text);
	}
	//---------------------------------------------------------------------------
	void ComboBox::InsertItem(int index, const Misc::AnsiString &text)
	{
		listBox->InsertItem(index, text);
	}
	//---------------------------------------------------------------------------
	void ComboBox::RemoveItem(int index)
	{
		listBox->RemoveItem(index);
	}
	//---------------------------------------------------------------------------
	void ComboBox::Clear()
	{
		Collapse();

		listBox->Clear();
	}
	//---------------------------------------------------------------------------
	bool ComboBox::Intersect(const Drawing::Point &point) const
	{
		if (Intersection::TestRectangle(absoluteLocation, size, point))
		{
			return true;
		}
		if (droppedDown && listBox->Intersect(point))
		{
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------
	/*void ComboBox::Invalidate()
	{
		//Button::Invalidate();

		dropDownRect = Drawing::Rectangle(bounds.GetLeft(), bounds.GetBottom() + 1, bounds.GetWidth(), 0);
		if (items.size() == 0)
		{
			dropDownRect.SetHeight(font->GetSize() + 4);
		}
		else
		{
			int height = items.size() * (font->GetSize() + 4) + 4;
			if (height > COMBOBOX_MAX_HEIGHT)
			{
				height = COMBOBOX_MAX_HEIGHT;
			}
			dropDownRect.SetHeight(height);
		}

		Drawing::Rectangle boundsBackup = bounds;
		bounds = dropDownRect;

		bounds = boundsBackup;

		//itemsRect = Drawing::Rectangle(dropDownRect.GetLeft() + 4, dropDownRect.GetTop() + 4, dropDownRect.GetWidth() - (scrollBar.GetVisible() ? scrollBar.GetWidth() : 0) - 8, dropDownRect.GetHeight() - 8);
		
		//scrollBar.SetPageSize(COMBOBOX_MAX_HEIGHT / (font->GetSize() + 2));
		//if (items.size() > 0)
		{
			//scrollBar.ShowItem(selectedIndex);
		}
	}*/
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void ComboBox::OnLostFocus()
	{
		ContainerControl::OnLostFocus();

		Collapse();
	}
	//---------------------------------------------------------------------------
	void ComboBox::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
		
		button->Render(renderer);
		
		if (droppedDown)
		{
			listBox->Render(renderer);
		}
	}
	//---------------------------------------------------------------------------
	//ComboBox::ComboBoxButton
	//---------------------------------------------------------------------------
	void ComboBox::ComboBoxButton::SetSize(const Drawing::Size &size)
	{
		realSize = size;

		Button::SetSize(size.InflateEx(-24, 0));
	}
	//---------------------------------------------------------------------------
	bool ComboBox::ComboBoxButton::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, realSize, point);
	}
	//---------------------------------------------------------------------------
	bool ComboBox::ComboBoxButton::OnKeyDown(const KeyboardMessage &keyboard)
	{
		return Control::OnKeyDown(keyboard);
	}
	//---------------------------------------------------------------------------
	void ComboBox::ComboBoxButton::Render(Drawing::IRenderer *renderer)
	{
		Drawing::Color tempColor = backColor;
		if (isFocused || isInside)
		{
			tempColor += mouseOverFocusColor;
		}
		
		renderer->SetRenderColor(tempColor);
		renderer->Fill(absoluteLocation.Left, absoluteLocation.Top + 1, realSize.Width, realSize.Height - 2);
		renderer->Fill(absoluteLocation.Left + 1, absoluteLocation.Top, realSize.Width - 2, realSize.Height);
		renderer->FillGradient(absoluteLocation.Left + 1, absoluteLocation.Top + 1, realSize.Width - 2, realSize.Height - 2, tempColor - Drawing::Color(0, 20, 20, 20));

		label->Render(renderer);

		int arrowLeft = absoluteLocation.Left + realSize.Width - 9;
		int arrowTop = absoluteLocation.Top + realSize.Height - 11;
		for (int i = 0; i < 4; ++i)
		{
			renderer->Fill(arrowLeft - i, arrowTop - i, 1 + i * 2, 1);
		}
	}
	//---------------------------------------------------------------------------
}