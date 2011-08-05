#ifndef __OSHGUI_GROUPBOX_H__
#define __OSHGUI_GROUPBOX_H__

#include "Control.h"

namespace OSHGui
{
	class GroupBox : public Control
	{
	public:
		GroupBox(Control *parent = NULL);
		virtual ~GroupBox();

		void SetText(const Misc::UnicodeString &text);
		const Misc::UnicodeString& GetText();
		
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		virtual Event::NextEventTypes ProcessMessage(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		Misc::UnicodeString text;
	};
}

#endif