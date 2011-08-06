#ifndef __OSHGUI_PROGRESSBAR_H__
#define __OSHGUI_PROGRESSBAR_H__

#include "Control.h"
#include "Panel.h"

namespace OSHGui
{
	class ProgressBar : public Control
	{	
	public:
		ProgressBar(Control *parent = NULL);
	
		void SetMin(unsigned int min);
		int GetMin();
		void SetMax(unsigned int max);
		int GetMax();
		void SetPosition(unsigned int position);
		int GetPosition();
		void SetBarColor(const Drawing::Color &color);
		const Drawing::Color& GetBarColor();

		virtual bool CanHaveFocus();
		virtual bool ContainsPoint(const Drawing::Point &point);

		virtual void Invalidate();
	
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		void Adjust();
	
		Drawing::Color barColor;
		unsigned int position,
					 min,
					 max;
	};
}

#endif