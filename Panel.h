#ifndef __OSHGUI_PANEL_H__
#define __OSHGUI_PANEL_H__

#include "Control.h"

namespace OSHGui
{
	class Panel : public Control
	{
	public:
		Panel(Control *parent = NULL);
		
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		virtual void Invalidate();

		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
	};
}

#endif