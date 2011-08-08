#ifndef __OSHGUI_LISTBOX_H__
#define __OSHGUI_LISTBOX_H__

#include <vector>
#include "Control.h"
#include "Panel.h"
#include "ScrollBar.h"
#include "ListItem.h"

#define LISTBOX_ITEM_HEIGHT 14

namespace OSHGui
{
	class ListBox : public Control
	{	
	public:
		ListBox(Control *parent = NULL);
		virtual ~ListBox();
		
		ListItem* GetItem(int index);
		int GetSelectedIndex();
		ListItem* GetSelectedItem();
		int GetItemsCount();
		
		virtual bool CanHaveFocus();
		virtual bool ContainsPoint(const Drawing::Point &point);
		bool AddItem(const Misc::UnicodeString &text);
		bool InsertItem(int index, const Misc::UnicodeString &text);
		bool RemoveItem(int index);
		bool Clear();
		void SelectItem(int newIndex);
		
		virtual void Invalidate();

		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
	
	protected:	
		ScrollBar scrollBar;
		
		int selectedIndex,
			firstVisibleItemIndex;
		bool drag;
		
		Drawing::Rectangle scrollBarRect,
						   itemsRect;
		
		std::vector<ListItem*> items;
	};
}

#endif