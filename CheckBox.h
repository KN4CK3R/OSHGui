#ifndef __OSHGUI_CHECKBOX_H__
#define __OSHGUI_CHECKBOX_H__

#include "Button.h"

namespace OSHGui
{
	class CheckBox : public Button
	{
	public:
		CheckBox(Panel *parentPanel = NULL);
		
		virtual void SetChecked(bool checked);
		virtual bool GetChecked();
				
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
	
	protected:
		virtual void UpdateRects();
		bool checked;
		
		Drawing::Rectangle buttonRect,
						   textRect;
	};
}

#endif