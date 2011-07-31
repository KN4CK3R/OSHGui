#ifndef __OSHGUI_BUTTON_H__
#define __OSHGUI_BUTTON_H__

#include "Label.h"

namespace OSHGui
{
	class Button : public Label
	{	
	public:
		Button(Control *parent = NULL);
		
		virtual bool CanHaveFocus();
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
	};
}

#endif