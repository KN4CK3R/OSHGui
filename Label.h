#ifndef __OSHGUI_LABEL_H__
#define __OSHGUI_LABEL_H__

#include "Control.h"
#include "Misc\TextHelper.h"

namespace OSHGui
{
	class Label : public Control
	{
	public:
		Label(Control *parent = NULL);
		
		void SetText(const Misc::UnicodeString &text);
		const Misc::UnicodeString& GetText();
		
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		virtual void Invalidate();
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:		
		bool pressed;
		Misc::UnicodeString text;
		Misc::TextHelper textHelper;
	};
}

#endif