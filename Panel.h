#ifndef __OSHGUI_PANEL_H__
#define __OSHGUI_PANEL_H__

#include "Control.h"

namespace OSHGui
{
	class Panel : public Control
	{
	public:
		Panel(Panel *parentPanel = NULL);
		
		virtual bool ContainsPoint(const Drawing::Point &point);

		void AddControl(Control *control);

		void CaptureMouse(Control *control);
		void ReleaseCapture();

		void RequestFocus(Control *control);
		void ClearFocus();

		void ClearRadioButtonGroup(int group);
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);

		Misc::List<Control*> Controls;
		
	protected:
		void OnMouseMove(const Drawing::Point &point);
		void OnMouseUp(const Drawing::Point &point);
	
		Control *FindControlAtPoint(const Drawing::Point &point);

		Control *focusControl,
				*captureControl,
				*mouseOverControl;
	};
}

#endif