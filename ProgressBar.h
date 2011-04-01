#ifndef __OSHGUI_PROGRESSBAR_H__
#define __OSHGUI_PROGRESSBAR_H__

#include "Control.h"
#include "Panel.h"

namespace OSHGui
{
	class ProgressBar : public Control
	{	
	public:
		ProgressBar(Panel *parentPanel = NULL);
	
		void SetMin(unsigned int min);
		int GetMin();
		void SetMax(unsigned int max);
		int GetMax();
		void SetPosition(unsigned int position);
		int GetPosition();
		
		virtual bool CanHaveFocus();
		virtual bool ContainsPoint(const Drawing::Point &point);
	
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		void Adjust();
	
		unsigned int position,
					 min,
					 max;
	};
}

#endif