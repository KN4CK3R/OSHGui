#ifndef __OSHGUI_FORM_H__
#define __OSHGUI_FORM_H__

#include "Panel.h"

namespace OSHGui
{
	class Form : public Panel
	{
	public:
		Form();

		virtual bool ContainsPoint(const Drawing::Point &point);
		
		void SetText(LPCWSTR text);
		LPCWSTR GetText();
		bool GetTextCopy(LPWSTR copy);
		
		void Show();

		Event::NextEventTypes ProcessEvent(Event *event);
		void Render(Drawing::IRenderer *renderer);

	protected:
		WCHAR text[256];
		Drawing::Rectangle captionBar;
	};
}

#endif