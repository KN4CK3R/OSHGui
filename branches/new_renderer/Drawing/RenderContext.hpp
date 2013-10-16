#ifndef OSHGUI_DRAWING_RENDERCONTEXT_HPP
#define OSHGUI_DRAWING_RENDERCONTEXT_HPP

#include "Point.hpp"
#include "RenderSurface.hpp"

namespace OSHGui
{
	class Control;

	namespace Drawing
	{
		class RenderContext
		{
		public:
			//! RenderingSurface to be used for drawing
			RenderSurface *Surface;
			//! The Window object that owns the RenederingSurface (0 for default root)
			const Control *Owner;
			//! The offset of the owning window on the root RenderingSurface.
			PointF Offset;
			//! The queue that rendering should be added to.
			RenderQueueType QueueType;
		};
	}
}

#endif
