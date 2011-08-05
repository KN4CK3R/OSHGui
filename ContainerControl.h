#ifndef __OSHGUI_CONTAINERCONTROL_H__
#define __OSHGUI_CONTAINERCONTROL_H__

#include "Control.h"
#include <vector>
#include <algorithm>

namespace OSHGui
{
	class ContainerControl
	{
	public:		
		void AddControl(Control *control);

		void ClearRadioButtonGroup(int group);
		
		Control *FindControlAtPoint(const Drawing::Point &point);
		Control *FindControlByName(const Misc::UnicodeString &name);
				
	protected:
		std::vector<Control*> Controls;
	};
}

#endif