#ifndef OSHGUI_RENDERTARGET_HPP
#define OSHGUI_RENDERTARGET_HPP

#include "Rectangle.hpp"
#include "Point.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class GeometryBuffer;
		class RenderQueue;

		/*!
		\brief
			Defines interface to some surface that can be rendered to.  Concrete
			instances of objects that implement the RenderTarget interface are
			normally created via the Renderer object.
		*/
		class OSHGUI_EXPORT RenderTarget
		{
		public:
			/*!
			\brief
				Draw geometry from the given GeometryBuffer onto the surface that
				this RenderTarget represents.

			\param buffer
				GeometryBuffer object holding the geometry that should be drawn to the
				RenderTarget.
			*/
			virtual void Draw(const GeometryBufferPtr &buffer) = 0;

			/*!
			\brief
				Draw geometry from the given RenderQueue onto the surface that
				this RenderTarget represents.

			\param queue
				RenderQueue object holding the geometry that should be drawn to the
				RenderTarget.
			*/
			virtual void Draw(const std::shared_ptr<RenderQueue> &queue) = 0;

			/*!
			\brief
				Set the area for this RenderTarget.  The exact action this function
				will take depends upon what the concrete class is representing.  For
				example, with a 'view port' style RenderTarget, this should set the area
				that the view port occupies on the display (or rendering window).

			\param area
				Rect object describing the new area to be assigned to the RenderTarget.

			\note
				When implementing this function, you should be sure to fire the event
				RenderTarget::EventAreaChanged so that interested parties can know that
				the change has occurred.

			\exception InvalidRequestException
				May be thrown if the RenderTarget does not support setting or changing
				its area, or if the area change can not be satisfied for some reason.
			*/
			virtual void SetArea(const RectangleF &area) = 0;

			/*!
			\brief
				Return the area defined for this RenderTarget.

			\return
				Rect object describing the currently defined area for this RenderTarget.
			*/
			virtual const RectangleF& GetArea() const = 0;

			/*!
			\brief
				Return whether the RenderTarget is an implementation that caches
				actual rendered imagery.

				Typically it is expected that texture based RenderTargets would return
				true in response to this call.  Other types of RenderTarget, like
				view port based targets, will more likely return false.

			\return
				- true if the RenderTarget does cache rendered imagery.
				- false if the RenderTarget does not cache rendered imagery.
			*/
			virtual bool IsImageryCache() const = 0;

			/*!
			\brief
				Activate the render target and put it in a state ready to be drawn to.

			\note
				You MUST call this before doing any rendering - if you do not call this,
				in the unlikely event that your application actually works, it will
				likely stop working in some future version.
			*/
			virtual void Activate() = 0;

			/*!
			\brief
				Deactivate the render target after having completed rendering.

			\note
				You MUST call this after you finish rendering to the target - if you do
				not call this, in the unlikely event that your application actually
				works, it will likely stop working in some future version.
			*/
			virtual void Deactivate() = 0;

			/*!
			\brief
				Take point \a p_in unproject it and put the result in \a p_out.
				Resulting point is local to GeometryBuffer \a buff.
			*/
			virtual void UnprojectPoint(const GeometryBufferPtr &buffer, const PointF &in, PointF &out) const = 0;
		};

		typedef std::shared_ptr<RenderTarget> RenderTargetPtr;
	}
}

#endif