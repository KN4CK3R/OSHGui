/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_DIRECT3D9RENDERTARGET_HPP
#define OSHGUI_DRAWING_DIRECT3D9RENDERTARGET_HPP

#include "Direct3D9Renderer.hpp"
#include "Direct3D9GeometryBuffer.hpp"
#include "../RenderQueue.hpp"
#include "../RenderTarget.hpp"
#include "../Rectangle.hpp"

#define NOMINMAX
#include <d3dx9.h>

namespace OSHGui
{
	namespace Drawing
	{
		template <typename T = RenderTarget>
		class Direct3D9RenderTarget : public T
		{
		public:
			Direct3D9RenderTarget(Direct3D9Renderer &_owner)
				: owner(_owner),
				  area(0, 0, 0, 0),
				  viewDistance(0),
				  matrixValid(false)
			{
			
			}
			//---------------------------------------------------------------------------
			void Draw(const GeometryBufferPtr &buffer) override
			{
				buffer->Draw();
			}
			//---------------------------------------------------------------------------
			void Draw(const RenderQueue &queue) override
			{
				queue.Draw();
			}
			//---------------------------------------------------------------------------
			void SetArea(const RectangleF &_area) override
			{
				area = _area;
				matrixValid = false;
			}
			//---------------------------------------------------------------------------
			const RectangleF& GetArea() const override
			{
				return area;
			}
			//---------------------------------------------------------------------------
			void Activate() override
			{
				if (!matrixValid)
				{
					UpdateMatrix();
				}

				D3DVIEWPORT9 vp;
				SetupViewport(vp);
				owner.GetDevice()->SetViewport(&vp);

				owner.GetDevice()->SetTransform(D3DTS_PROJECTION, &matrix);
			}
			//---------------------------------------------------------------------------
			void Deactivate() override
			{
			
			}
			//---------------------------------------------------------------------------
			void UnprojectPoint(const GeometryBufferPtr &buffer, const PointF &in, PointF &out) const override
			{
				if (!matrixValid)
				{
					UpdateMatrix();
				}

				auto gb = std::static_pointer_cast<Direct3D9GeometryBuffer>(buffer);
				
				D3DVIEWPORT9 vp;
				SetupViewport(vp);

				D3DXVECTOR3 in_vec;
				in_vec.z = 0.0f;

				D3DXVECTOR3 p1;
				D3DXVECTOR3 p2;
				D3DXVECTOR3 p3;
				in_vec.x = 0;
				in_vec.y = 0;
				D3DXVec3Project(&p1, &in_vec, &vp, &matrix, 0, gb->GetMatrix());

				in_vec.x = 1;
				in_vec.y = 0;
				D3DXVec3Project(&p2, &in_vec, &vp, &matrix, 0, gb->GetMatrix());

				in_vec.x = 0;
				in_vec.y = 1;
				D3DXVec3Project(&p3, &in_vec, &vp, &matrix, 0, gb->GetMatrix());

				D3DXPLANE surface_plane;
				D3DXPlaneFromPoints(&surface_plane, &p1, &p2, &p3);

				in_vec.x = vp.Width * 0.5f;
				in_vec.y = vp.Height * 0.5f;
				in_vec.z = -viewDistance;
				D3DXVECTOR3 t1;
				D3DXVec3Unproject(&t1, &in_vec, &vp, &matrix, 0, gb->GetMatrix());

				in_vec.x = in.X;
				in_vec.y = in.Y;
				in_vec.z = 0.0f;
				D3DXVECTOR3 t2;
				D3DXVec3Unproject(&t2, &in_vec, &vp, &matrix, 0, gb->GetMatrix());

				D3DXVECTOR3 intersect;
				D3DXPlaneIntersectLine(&intersect, &surface_plane, &t1, &t2);

				out.X = intersect.x;
				out.Y = intersect.y;
			}

		protected:
			void UpdateMatrix() const
			{
				const float fov = 0.523598776f;
				const float width = area.GetWidth();
				const float height = area.GetHeight();
				const float aspect = width / height;
				const float halfWidth = width * 0.5f;
				const float halfHeight = height * 0.5f;
				viewDistance = halfWidth / (aspect * 0.267949192431123f);

				D3DXVECTOR3 eye(halfWidth, halfHeight, -viewDistance);
				D3DXVECTOR3 at(halfWidth, halfHeight, 1);
				D3DXVECTOR3 up(0, -1, 0);

				D3DXMATRIX tmp;
				D3DXMatrixMultiply(&matrix, D3DXMatrixLookAtRH(&matrix, &eye, &at, &up), D3DXMatrixPerspectiveFovRH(&tmp, fov, aspect, viewDistance * 0.5f, viewDistance * 2.0f));

				matrixValid = true;
			}
			//---------------------------------------------------------------------------
			void SetupViewport(D3DVIEWPORT9 &viewport) const
			{
				viewport.X = area.GetLeft();
				viewport.Y = area.GetTop();
				viewport.Width = area.GetWidth();
				viewport.Height = area.GetHeight();
				viewport.MinZ = 0.0f;
				viewport.MaxZ = 1.0f;
			}
			//---------------------------------------------------------------------------

			Direct3D9Renderer& owner;
			
			RectangleF area;
			
			mutable D3DXMATRIX matrix;
			mutable bool matrixValid;
			mutable float viewDistance;
		};
	}
}

#endif