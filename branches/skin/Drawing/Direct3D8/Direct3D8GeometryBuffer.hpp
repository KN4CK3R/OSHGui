/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_DIRECT3D8GEOMETRYBUFFER_HPP
#define OSHGUI_DRAWING_DIRECT3D8GEOMETRYBUFFER_HPP

#include "../GeometryBuffer.hpp"
#include "Direct3D8Renderer.hpp"
#include "Direct3D8X.hpp"
#include <vector>

namespace OSHGui
{
	namespace Drawing
	{
		class Direct3D8Texture;

		/**
		 * Die Direct3D8 Variante des GeometryBuffer.
		 */
		class OSHGUI_EXPORT Direct3D8GeometryBuffer : public GeometryBuffer
		{
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * \param owner Renderer Objekt, das den GeometryBuffer erstellt hat
			 */
			Direct3D8GeometryBuffer(Direct3D8Renderer &owner);

			virtual void SetTranslation(const Vector &translation) override;
			virtual void SetRotation(const Quaternion &rotation) override;
			virtual void SetPivot(const Vector &pivot) override;
			virtual void SetClippingRegion(const RectangleF &region) override;
			virtual void SetActiveTexture(const TexturePtr &texture) override;
			virtual void SetClippingActive(const bool active) override;
			virtual bool IsClippingActive() const override;
			virtual void AppendVertex(const Vertex &vertex) override;
			virtual void AppendGeometry(const Vertex *const vertices, uint32_t count) override;
			virtual void Draw() const override;
			virtual void Reset() override;

		protected:
			/**
			 * Erledigt das Batch Management bevor neue Geometrie hinzugef�gt wird.
			 */
			void PerformBatchManagement();
			/**
			 * Updatet die Transformationsmatrix vor dem Zeichnen.
			 */
			void UpdateMatrix() const;

			struct D3DVertex
			{
				D3DVertex(float _x, float _y, float _z, DWORD _color, float _tu, float _tv)
					: x(_x),
					  y(_y),
					  z(_z),
					  color(_color),
					  tu(_tu),
					  tv(_tv)
				{

				}

				float x;
				float y;
				float z;
				DWORD color;
				float tu;
				float tv;
			};

			struct BatchInfo
			{
				BatchInfo(LPDIRECT3DTEXTURE8 _texture, uint32_t _count, VertexDrawMode _mode, bool _clip)
					: texture(_texture),
					  count(_count),
					  mode(_mode),
					  clip(_clip)
				{

				}

				LPDIRECT3DTEXTURE8 texture;
				uint32_t count;
				VertexDrawMode mode;
				bool clip;
			};

			Direct3D8Renderer& owner;
			std::shared_ptr<Direct3D8Texture> activeTexture;
			
			typedef std::vector<BatchInfo> BatchList;
			BatchList batches;
			
			typedef std::vector<D3DVertex> VertexList;
			VertexList vertices;
			
			bool clippingActive;
			RectangleF clipRect;
			
			Vector translation;
			Quaternion rotation;
			Vector pivot;
			
			mutable bool matrixValid;
			mutable D3DXMATRIX matrix;
		};
	}
}

#endif