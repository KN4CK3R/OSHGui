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

		void SetText(LPCSTR text);
		LPCSTR GetText();
		bool GetTextCopy(LPSTR copy);
		
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		virtual Event::NextEventTypes ProcessMessage(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		char text[256];
	};
}

#endif