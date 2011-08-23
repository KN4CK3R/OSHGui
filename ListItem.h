#ifndef __OSHGUI_LISTITEM_H__
#define __OSHGUI_LISTITEM_H__

#include "Drawing\Rectangle.h"
#include "Misc\Strings.h"

namespace OSHGui
{
	class ListItem
	{
	public:
		Misc::UnicodeString Text;
		void *Data;
	};
}

#endif