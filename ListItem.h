#ifndef __OSHGUI_LISTITEM_H__
#define __OSHGUI_LISTITEM_H__

#include "Drawing\Rectangle.h"
#include "Misc\Strings.h"
#include "Misc\Any.h"

namespace OSHGui
{
	/**
	 * Diese Klasse repräsentiert einen Listeintrag.
	 */
	class ListItem
	{
	public:
		Misc::UnicodeString Text;
		Misc::Any Data;
	};
}

#endif