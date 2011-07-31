#ifndef __OSHGUI_GROUPBOX_H__
#define __OSHGUI_GROUPBOX_H__

#include "Panel.h"

namespace OSHGui
{
	class GroupBox : public Panel
	{
	public:
		GroupBox(Control *parent = NULL);
		virtual ~GroupBox();

		void SetText(const char *text);
		const char* GetText();
		bool GetTextCopy(char *copy);
		
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		virtual Event::NextEventTypes ProcessMessage(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		char text[256];
	};
}

#endif