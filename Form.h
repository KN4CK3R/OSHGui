#ifndef __OSHGUI_FORM_H__
#define __OSHGUI_FORM_H__

#include "Control.h"
#include "Misc\TextHelper.h"

namespace OSHGui
{
	class Form : public Control
	{
	public:
		Form();
		virtual ~Form();

		virtual bool ContainsPoint(const Drawing::Point &point);
		
		void SetText(const Misc::UnicodeString &text);
		const Misc::UnicodeString& GetText();
		
		void Show();

		virtual void Invalidate();

		Event::NextEventTypes ProcessEvent(Event *event);
		void Render(Drawing::IRenderer *renderer);

	protected:
		Misc::TextHelper textHelper;
		Misc::UnicodeString text;
		Drawing::Rectangle captionBar,
						   closeRect;
		bool drag;
	};
}

#endif