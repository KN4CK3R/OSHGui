#ifndef __OSHGUI_LISTITEM_H__
#define __OSHGUI_LISTITEM_H__

namespace OSHGui
{
	class ListItem
	{
	public:
		char Text[256];
		void *Data;
		Drawing::Rectangle ItemRect;
	};
}

#endif