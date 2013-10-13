#ifndef OSHGUI_DRAWING_GEOMETRYBUFFER_HPP
#define OSHGUI_DRAWING_GEOMETRYBUFFER_HPP

#include "Renderer.hpp"
#include "Rectangle.hpp"
#include "Vertex.hpp"
#include "Quaternion.hpp"
#include "Vector.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class OSHGUI_EXPORT GeometryBuffer
		{
		public:
			//! Destructor
			virtual ~GeometryBuffer();

			/*!
			\brief
				Draw the geometry buffered within this GeometryBuffer object.
			*/
			virtual void Draw() const = 0;

			/*!
			\brief
				Set the translation to be applied to the geometry in the buffer when it
				is subsequently rendered.

			\param v
				Vector3 describing the three axis translation vector to be used.
			*/
			virtual void SetTranslation(const Vector &translation) = 0;

			/*!
			\brief
				Set the rotations to be applied to the geometry in the buffer when it is
				subsequently rendered.

			\param r
				Quaternion describing the rotation to be used.
			*/
			virtual void SetRotation(const Quaternion &rotation) = 0;

			/*!
			\brief
				Set the pivot point to be used when applying the rotations.

			\param p
				Vector3 describing the location of the pivot point to be used when
				applying the rotation to the geometry.
			*/
			virtual void SetPivot(const Vector &pivot) = 0;

			/*!
			\brief
				Set the clipping region to be used when rendering this buffer.
			*/
			virtual void SetClippingRegion(const RectangleF &region) = 0;

			/*!
			\brief
				Append a single vertex to the buffer.

			\param vertex
				Vertex object describing the vertex to be added to the GeometryBuffer.
			*/
			virtual void AppendVertex(const Vertex &vertex) = 0;

			/*!
			\brief
				Append a number of vertices from an array to the GeometryBuffer.

			\param vbuff
				Pointer to an array of Vertex objects that describe the vertices that
				are to be added to the GeometryBuffer.

			\param vertex_count
				The number of Vertex objects from the array \a vbuff that are to be
				added to the GeometryBuffer.
			*/
			virtual void AppendGeometry(const Vertex *const vertices, std::uint32_t count) = 0;

			/*!
			\brief
				Set the active texture to be used with all subsequently added vertices.

			\param texture
				Pointer to a Texture object that shall be used for subsequently added
				vertices.  This may be 0, in which case texturing will be disabled for
				subsequently added vertices.
			*/
			virtual void SetActiveTexture(TexturePtr &texture) = 0;

			/*!
			\brief
				Clear all buffered data and reset the GeometryBuffer to the default
				state.
			*/
			virtual void Reset() = 0;

			/*!
			\brief
				Return a pointer to the currently active Texture object.  This may
				return 0 if no texture is set.

			\return
				Pointer the Texture object that is currently active, or 0 if texturing
				is not being used.
			*/
			virtual TexturePtr GetActiveTexture() const = 0;

			/*!
			\brief
				Return the total number of vertices currently held by this
				GeometryBuffer object.

			\return
				The number of vertices that have been appended to this GeometryBuffer.
			*/
			virtual std::uint32_t GetVertexCount() const = 0;

			/*!
			\brief
				Return the number of batches of geometry that this GeometryBuffer has
				split the vertices into.

			\note
				How batching is done will be largely implementation specific, although
				it would be reasonable to expect that you will have <em>at least</em>
				one batch of geometry per texture switch.

			\return
				The number of batches of geometry held by the GeometryBuffer.
			*/
			virtual std::uint32_t GetBatchCount() const = 0;

			/*!
			\brief
				Set the blend mode option to use when rendering this GeometryBuffer.

			\note
				The blend mode setting is not a 'state' setting, but is used for \e all
				geometry added to the buffer regardless of when the blend mode is set.

			\param mode
				One of the BlendMode enumerated values indicating the blending mode to
				be used.
			*/
			virtual void SetBlendMode(const BlendMode mode);

			/*!
			\brief
				Return the blend mode that is set to be used for this GeometryBuffer.

			\return
				One of the BlendMode enumerated values indicating the blending mode
				that will be used when rendering all geometry added to this
				GeometryBuffer object.
			*/
			virtual BlendMode GetBlendMode() const;

			/*!
			\brief
				Set whether clipping will be active for subsequently added vertices.

			\param active
				- true if vertices added after this call should be clipped to the
				  clipping region defined for this GeometryBuffer.
				- false if vertices added after this call should not be clipped
				  (other than to the edges of rendering target.
			*/
			virtual void SetClippingActive(const bool active) = 0;

			/*
			\brief
				Return whether clipping will be used for the current batch
				of vertices being defined.

			\return
				- true if vertices subsequently added to the GeometryBuffer will
				  be clipped to the clipping region defined for this GeometryBuffer.
				- false if vertices subsequently added will not be clippled (other than
				  to the edges of the rendering target).
			*/
			virtual bool IsClippingActive() const = 0;

		protected:
			//! Constructor.
			GeometryBuffer();

			//! The BlendMode to use when rendering this GeometryBuffer.
			BlendMode blendMode;
		};

		typedef std::shared_ptr<GeometryBuffer> GeometryBufferPtr;
	}
}

#endif