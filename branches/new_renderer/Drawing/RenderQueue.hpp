#ifndef OSHGUI_DRAWING_RENDERQUEUE_HPP
#define OSHGUI_DRAWING_RENDERQUEUE_HPP

#include <vector>
#include "GeometryBuffer.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/*!
		\brief
			Class that represents a queue of GeometryBuffer objects to be rendered.

		\note
			The RenderQueue does not make copies of added GeometryBuffers, nor does it
			take ownership of them - it is up to other parts of the system to manage the
			lifetime of the GeometryBuffer objects (and to remove them from any
			RenderQueue to which they may be attached prior to destoying them).
		*/

		
		class OSHGUI_EXPORT RenderQueue
		{
		public:
			/*!
			\brief
				Draw all GeometryBuffer objects currently listed in the RenderQueue.
				The GeometryBuffer objects remain in the queue after drawing has taken
				place.
			*/
			void Draw() const;

			/*!
			\brief
				Add a GeometryBuffer to the RenderQueue.  Ownership of the
				GeometryBuffer does not pass to the RenderQueue.

			\param buffer
				GeometryBuffer that is to be added to the RenderQueue for later drawing.
			*/
			void AddGeometryBuffer(const GeometryBufferPtr &buffer);

			/*!
			\brief
				Remove a GeometryBuffer previously queued for drawing.  If the specified
				GeometryBuffer is not added to the queue, no action is taken.  The
				removed GeometryBuffer is not destroyed or modified in any way.

			\param buffer
				GeometryBuffer to be removed from the queue.
			*/
			void RemoveGeometryBuffer(const GeometryBufferPtr &buffer);

			/*!
			\brief
				Remove any and all queued GeometryBuffer objects and restore the queue
				to the default state.  Any GeometryBuffer objects removed are not
				destroyed or modified in any way.
			*/
			void Reset();

		private:
			//! Type to use for the GeometryBuffer collection.
			typedef std::vector<GeometryBufferPtr> BufferList;
			//! Collection of GeometryBuffer objects that comprise this RenderQueue.
			BufferList buffers;
		};

		typedef std::shared_ptr<RenderQueue> RenderQueuePtr;
	}
}

#endif
