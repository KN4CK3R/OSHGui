#ifndef __OSHGUI_WAITCURSOR_H__
#define __OSHGUI_WAITCURSOR_H__

#include <vector>
#include "Cursor.h"
#include "..\Misc\DateTime.h"

namespace OSHGui
{
	class WaitCursor : public Cursor
	{
	public:
		WaitCursor();
		virtual ~WaitCursor();
		
		virtual void Render(Drawing::IRenderer *renderer, const Drawing::Point &cursorPosition);
		
	protected:
		virtual void CreateCursor();
		
	private:
		std::vector<std::shared_ptr<Drawing::ITexture> > frames;
		int totalFrames;
		static const Misc::TimeSpan interval;
		Misc::DateTime nextFrameTime;
		int nextFrame;
	};
}

#endif