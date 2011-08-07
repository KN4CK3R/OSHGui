#ifndef __OSHGUI_SCROLLBAR_H__
#define __OSHGUI_SCROLLBAR_H__

#include "Control.h"

#define SCROLLBAR_MIN_SLIDER_HEIGHT 25
#define SCROLLBAR_DEFAULT_BOUNDS_WIDTH 14
#define SCROLLBAR_DEFAULT_BUTTON_WIDTH 14
#define SCROLLBAR_DEFAULT_BUTTON_HEIGHT 18

namespace OSHGui
{
	class ScrollBar : public Control
	{
	public:
		ScrollBar(Control *parent);
		
		void SetRange(int start, int end);
		void SetPosition(int position);
		int GetPosition();
		void SetPageSize(int pageSize);
		int GetPageSize();

		virtual bool ContainsPoint(const Drawing::Point &point);
		
		void Scroll(int delta);
		void ShowItem(int index);

		virtual void Invalidate();
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
				
	protected:
		void UpdateSliderRect();
		void Capture();
	
		int start,
			end,
			position,
			pageSize;
		bool drag,
			 showScrollBar;
		
		Drawing::Rectangle upButtonRect,
						   downButtonRect,
						   trackRect,
						   sliderRect;
				  
		long delayTimestamp;
	};
}

#endif