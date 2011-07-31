#ifndef __OSHGUI_SCROLLBAR_H__
#define __OSHGUI_SCROLLBAR_H__

#include "Control.h"

#define SCROLLBAR_MIN_SLIDER_HEIGHT 20
#define SCROLLBAR_DEFAULT_BOUNDS_WIDTH 19
#define SCROLLBAR_DEFAULT_BUTTON_WIDTH 13
#define SCROLLBAR_DEFAULT_BUTTON_HEIGHT 19

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
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);
				
	protected:
		void UpdateRects();
		void UpdateSliderRect();
		void Capture();
	
		int start,
			end,
			position,
			pageSize;
		bool drag,
			 showSlider;
		
		Drawing::Rectangle upButtonRect,
						   downButtonRect,
						   trackRect,
						   sliderRect;
				  
		long delayTimestamp;
	};
}

#endif