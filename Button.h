#ifndef __OSHGUI_BUTTON_H__
#define __OSHGUI_BUTTON_H__

#include "LinkLabel.h"

namespace OSHGui
{
	class Button : public LinkLabel
	{	
	public:
		Button(Panel *parentPanel = NULL);
		
		virtual bool CanHaveFocus();
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);

	protected:
		Drawing::Color backColorDiff,
					   borderColorDiff;
	};
}

#endif