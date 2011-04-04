#ifndef __OSHGUI_GROUPBOX_H__
#define __OSHGUI_GROUPBOX_H__

#include "Panel.h"

namespace OSHGui
{
	class GroupBox : public Panel
	{
	public:
		GroupBox(Panel *parentPanel = NULL);
		virtual ~GroupBox();
		
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		virtual Event::NextEventType ProcessMessage(Event *event);
		virtual void Render(IRenderer *renderer);
		
	protected:
		char text[256];
	};
}

#endif