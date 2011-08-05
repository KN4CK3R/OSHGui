#ifndef __OSHGUI_FORM_H__
#define __OSHGUI_FORM_H__

#include "Control.h"

namespace OSHGui
{
	class Form : public Control
	{
	public:
		Form();

		virtual bool ContainsPoint(const Drawing::Point &point);
		
		void SetText(const Misc::UnicodeString &text);
		const Misc::UnicodeString& GetText();
		
		void Show();

		virtual void Invalidate();

		virtual Drawing::Point PointToClient(const Drawing::Point &point);
		virtual Drawing::Point PointToScreen(const Drawing::Point &point);

		Event::NextEventTypes ProcessEvent(Event *event);
		void Render(Drawing::IRenderer *renderer);

	protected:	
		Misc::UnicodeString text;
		Drawing::Rectangle captionBar,
						   closeRect,
						   minimizeRect;
		bool drag;
	};
}

#endif