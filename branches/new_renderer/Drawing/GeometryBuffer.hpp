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
		/**
		 * Die Klasse speichert Geometrien zum Zeichnen.
		 */
		class OSHGUI_EXPORT GeometryBuffer
		{
		public:
			/**
			 * Destructor der Klasse.
			 */
			virtual ~GeometryBuffer();

			/**
			 * Zeichnet die gespeicherte Geometrie.
			 */
			virtual void Draw() const = 0;

			/**
			 * Legt die Position des GeometryBuffer im Raum fest.
			 *
			 * @param translation
			 */
			virtual void SetTranslation(const Vector &translation) = 0;

			/**
			 * Legt die Rotation des GeometryBuffer fest.
			 *
			 * @param rotation
			 */
			virtual void SetRotation(const Quaternion &rotation) = 0;

			/**
			 * Legt das Pivot Element (Stelle im Raum, um die der GeometryBuffer gedreht wird) fest.
			 *
			 * @param pivot
			 */
			virtual void SetPivot(const Vector &pivot) = 0;

			/**
			 * Legt die Clipping Region fest.
			 *
			 * @param region
			 */
			virtual void SetClippingRegion(const RectangleF &region) = 0;

			/**
			 * Fügt einen einzelnen Vertex zum GeometryBuffer hinzu.
			 *
			 * @param vertex
			 */
			virtual void AppendVertex(const Vertex &vertex) = 0;

			/**
			 * Fügt mehrere Vertices zum GeometryBuffer hinzu.
			 *
			 * @param vertices Zeiger auf ein Vertex Array
			 * @param count Anzahl der Vertices
			 */
			virtual void AppendGeometry(const Vertex *const vertices, uint32_t count) = 0;

			/**
			 * Legt die momentan aktive Textur zum Zeichnen fest.
			 *
			 * @param texture
			 */
			virtual void SetActiveTexture(TexturePtr &texture) = 0;

			/**
			 * Löscht alle gespeicherten Vertices aus dem GeometryBuffer.
			 */
			virtual void Reset() = 0;

			/**
			 * Ruft die momentan aktive Textur ab.
			 *
			 * @return die aktive Textur oder nullptr
			 */
			virtual TexturePtr GetActiveTexture() const = 0;

			/*!
			\brief
				Return the total number of vertices currently held by this
				GeometryBuffer object.

			\return
				The number of vertices that have been appended to this GeometryBuffer.
			*/
			virtual uint32_t GetVertexCount() const = 0;

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
			virtual uint32_t GetBatchCount() const = 0;

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

			/**
			 * Legt fest, ob die anschließend hinzugefügten Vertices geclippt werden sollen.
			 *
			 * @param active
			 */
			virtual void SetClippingActive(const bool active) = 0;

			/**
			 * Ruft ab, ob derzeit Clipping aktiv ist.
			 *
			 * @return Clipping aktiv
			 */
			virtual bool IsClippingActive() const = 0;

		protected:
			GeometryBuffer();

			BlendMode blendMode;
		};

		typedef std::shared_ptr<GeometryBuffer> GeometryBufferPtr;
	}
}

#endif