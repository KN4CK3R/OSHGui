#ifndef __OSHGUI_COMBOBOX_H__
#define __OSHGUI_COMBOBOX_H__

#include "Button.h"
#include "ScrollBar.h"
#include "ListItem.h"

#define COMBOBOX_ITEM_HEIGHT 22
#define COMBOBOX_MAX_HEIGHT 220

namespace OSHGui
{
	class ComboBox : public Button
	{	
	public:
		ComboBox(Control *parent = NULL);
		virtual ~ComboBox();
		
		ListItem* GetItem(int index);
		int GetSelectedIndex();
		ListItem* GetSelectedItem();
		int GetItemsCount();
		
		virtual bool CanHaveFocus();
		virtual bool ContainsPoint(const Drawing::Point &point);
		bool AddItem(const char *itemText);
		bool InsertItem(int index, const char *itemText);
		bool RemoveItem(int index);
		bool Clear();
		bool Contains(const char *text);
		int Find(const char *text);
		void SelectItem(int newIndex);
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		virtual void OnFocusOut();
	
	protected:
		virtual void UpdateRects();
	
		ScrollBar scrollBar;
		
		int dropdownHeight;
		
		int selectedIndex,
			focusedIndex;
		bool opened;
		
		Drawing::Rectangle buttonRect,
						   textRect,
						   dropdownRect,
						   scrollBarRect,
						   itemsRect;
		
		Misc::List<ListItem*> items;
	};
}

#endif