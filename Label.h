#ifndef __OSHGUI_LABEL_H__
#define __OSHGUI_LABEL_H__

#include "Control.h"
#include "Panel.h"

namespace OSHGui
{
	class Label : public Control
	{
	public:
		Label(Panel *parentPanel = NULL);
		
		void SetText(const char *text);
		const char* GetText();
		bool GetTextCopy(char *copy);
		
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		char text[256]; 
	};
}

#endif