#ifndef __OSHGUI_CHECKBOX_H__
#define __OSHGUI_CHECKBOX_H__

#include "Label.h"

namespace OSHGui
{
	class CheckBox : public Label
	{
	public:
		CheckBox(Control *parent = NULL);
		
		virtual void SetChecked(bool checked);
		virtual bool GetChecked();
		
		virtual void Invalidate();
				
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
	
	protected:
		bool checked;
	};
}

#endif