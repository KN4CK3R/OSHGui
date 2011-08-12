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
		bool AddItem( const Misc::UnicodeString &text);
		bool InsertItem(int index, const Misc::UnicodeString &text);
		bool RemoveItem(int index);
		bool Clear();
		void SelectItem(int newIndex);

		virtual void Invalidate();
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
	private:
		void CloseOnLostFocus();
	
	protected:	
		ScrollBar scrollBar;
		
		int dropdownHeight;
		
		int selectedIndex,
			firstVisibleItemIndex,
			mouseOverItemIndex;
		bool open;
		
		Drawing::Rectangle dropDownRect,
						   itemsRect;
		
		std::vector<ListItem*> items;
	};
}

#endif