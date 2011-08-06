#ifndef __OSHGUI_GROUPBOX_H__
#define __OSHGUI_GROUPBOX_H__

#include "Control.h"
#include "Misc\TextHelper.h"

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

		virtual void Invalidate();
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		Misc::TextHelper textHelper;

		Drawing::Rectangle renderBounds;
	};
}

#endif