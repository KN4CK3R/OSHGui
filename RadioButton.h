#ifndef __OSHGUI_RADIOBUTTON_H__
#define __OSHGUI_RADIOBUTTON_H__

#include "CheckBox.h"

namespace OSHGui
{
	class RadioButton : public CheckBox
	{	
	public:
		RadioButton(Control *parent = NULL);
		
		virtual void SetChecked(bool checked);
		void SetGroup(int group);
		int GetGroup();
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		virtual void SetCheckedInternal(bool checked);
	
		int group;		
	};
}

#endif