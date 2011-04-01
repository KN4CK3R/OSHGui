#ifndef __OSHGUI_LINKLABEL_H__
#define __OSHGUI_LINKLABEL_H__

#include "Label.h"

namespace OSHGui
{
	class LinkLabel : public Label
	{
	public:
		LinkLabel(Panel *parentPanel = NULL);
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		bool pressed;
	};
}

#endif