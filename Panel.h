#ifndef __OSHGUI_PANEL_H__
#define __OSHGUI_PANEL_H__

#include "Control.h"
#include <vector>
#include <algorithm>

namespace OSHGui
{
	class Panel : public Control
	{
	public:
		Panel(Control *parent = NULL);
		
		virtual bool ContainsPoint(const Drawing::Point &point);

		void AddControl(Control *control);

		void ClearRadioButtonGroup(int group);
		
		Control *FindControlAtPoint(const Drawing::Point &point);
		Control *FindControlByName(const Misc::UnicodeString &name);
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		std::vector<Control*> Controls;
		//Misc::List<Control*> Controls;
	};
}

#endif