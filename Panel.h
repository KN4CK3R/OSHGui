#ifndef __OSHGUI_PANEL_H__
#define __OSHGUI_PANEL_H__

#include "Control.h"
#include "ContainerControl.h"

namespace OSHGui
{
	class Panel : public Control, ContainerControl
	{
	public:
		Panel(Control *parent = NULL);
		
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
	};
}

#endif