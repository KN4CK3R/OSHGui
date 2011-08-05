#ifndef __OSHGUI_CONTAINERCONTROL_H__
#define __OSHGUI_CONTAINERCONTROL_H__

#include "Control.h"
#include <vector>
#include <algorithm>

namespace OSHGui
{
	class ContainerControl : public Control
	{
	public:
		ContainerControl(Control *parent);

		void AddControl(Control *control);

		void ClearRadioButtonGroup(int group);

		void RequestFocus(Control *control);
		void ClearFocus();
		void CaptureMouse(Control *control);
		void ReleaseCapture();

		void InvalidateChildren();
		
		Control* FindControlAtPoint(const Drawing::Point &point);
		Control* FindControlByName(const Misc::UnicodeString &name);
				
	protected:
		
	};
}

#endif