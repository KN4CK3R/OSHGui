#ifndef __OSHGUI_TRACKBAR_H__
#define __OSHGUI_TRACKBAR_H__

#include "Control.h"

#define TRACKBAR_SLIDER_WIDTH 8
#define TRACKBAR_SLIDER_HEIGHT 16

namespace OSHGui
{
	class TrackBar : public Control
	{
	public:
		TrackBar(Control *parent);
		
		virtual bool CanHaveFocus();
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		void SetRange(int min, int max);
		void SetValue(int value);
		int GetValue();

		virtual void Invalidate();
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);

	protected:	
		virtual void SetValueInternal(int value);
		virtual int ValueFromPosition(int position);
	
		int min,
			max,
			value,
			sliderMiddle,
			dragX,
			dragOffset;
		bool pressed;
		
		Drawing::Rectangle sliderRect;
	};
}

#endif