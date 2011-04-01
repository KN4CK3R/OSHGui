#ifndef __OSHGUI_TRACKBAR_H__
#define __OSHGUI_TRACKBAR_H__

#include "Control.h"
#include "Panel.h"

#define TRACKBAR_SLIDER_WIDTH 13
#define TRACKBAR_SLIDER_HEIGHT 22

namespace OSHGui
{
	class TrackBar : public Control
	{
	public:
		TrackBar(Panel *parentPanel);
		
		virtual bool CanHaveFocus();
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		void SetRange(int min, int max);
		void SetValue(int value);
		int GetValue();
		
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		virtual void Render(Drawing::IRenderer *renderer);

	protected:
		virtual void UpdateRects();
	
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