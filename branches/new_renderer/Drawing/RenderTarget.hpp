/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

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

		/**
		 * Interface f�r Zeichenoberfl�chen.
		 */
		class OSHGUI_EXPORT RenderTarget
		{
		public:
			/**
			 * Zeichnet den Inhalt des GeometryBuffer.
			 *
			 * \param buffer
			 */
			virtual void Draw(const GeometryBufferPtr &buffer) = 0;

			/**
			 * Zeichnet den Inhalt der RenderQueue.
			 *
			 * \param queue
			 */
			virtual void Draw(const RenderQueue &queue) = 0;

			/**
			 * Legt den Zeichenbereich fest.
			 *
			 * \param area Zeichenbereich
			 */
			virtual void SetArea(const RectangleF &area) = 0;

			/**
			 * Ruft den Zeichenbereich ab.
			 *
			 * \return Zeichenbereich
			 */
			virtual const RectangleF& GetArea() const = 0;

			/**
			 * Ruft ab, ob das RenderTarget den gezeichneten Inhalt �ber mehrere Frames hinweg beh�lt (z.B. Zeichnen auf eine Textur)
			 *
			 * \return true, falls der Inhalt gecacht wird
			 */
			virtual bool IsImageryCache() const = 0;

			/**
			 * Aktiviert das RenderTarget, damit auf es gezeichnet werden kann.
			 */
			virtual void Activate() = 0;

			/**
			 * Deaktiviert das RenderTarget nach dem Zeichnen.
			 */
			virtual void Deactivate() = 0;

			virtual void UnprojectPoint(const GeometryBufferPtr &buffer, const PointF &in, PointF &out) const = 0;
		};

		typedef std::shared_ptr<RenderTarget> RenderTargetPtr;
	}
}

#endif