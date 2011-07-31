#ifndef __OSHGUI_LISTBOX_H__
#define __OSHGUI_LISTBOX_H__

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
		bool AddItem(const char *text);
		bool InsertItem(int index, const char *text);
		bool RemoveItem(int index);
		bool Clear();
		void SelectItem(int newIndex);
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
	
	protected:
		virtual void UpdateRects();
	
		ScrollBar scrollBar;
		
		int selectedIndex;
		bool drag;
		
		Drawing::Rectangle scrollBarRect,
						   itemsRect;
		
		Misc::List<ListItem*> items;
	};
}

#endif