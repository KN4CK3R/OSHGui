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
		
		void SetText(const char *text);
		const char* GetText();
		bool GetTextCopy(char *copy);
		
		void Show();

		virtual Drawing::Point PointToClient(const Drawing::Point &point);
		virtual Drawing::Point PointToScreen(const Drawing::Point &point);

		Event::NextEventTypes ProcessEvent(Event *event);
		void Render(Drawing::IRenderer *renderer);

	protected:
		virtual void UpdateRects();
	
		char text[256];
		Drawing::Rectangle captionBar,
						   closeRect,
						   minimizeRect,
						   clientArea;
		bool drag;
	};
}

#endif