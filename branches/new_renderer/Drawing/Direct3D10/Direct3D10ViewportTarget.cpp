#include "Direct3D10ViewportTarget.hpp"
#include "../RenderQueue.hpp"
#include "../GeometryBuffer.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		Direct3D10ViewportTarget::Direct3D10ViewportTarget(Direct3D10Renderer &owner)
			: Direct3D10RenderTarget(owner)
		{
			D3D10_VIEWPORT vp;
			uint32_t count = 1;
			owner.GetDevice()->RSGetViewports(&count, &vp);
			
			if (count != 1)
			{
				throw;
			}

			SetArea(RectangleF(PointF(vp.TopLeftX, vp.TopLeftY), SizeF(vp.Width, vp.Height)));
		}
		//---------------------------------------------------------------------------
		Direct3D10ViewportTarget::Direct3D10ViewportTarget(Direct3D10Renderer &owner, const RectangleF &area)
			: Direct3D10RenderTarget(owner)
		{
			SetArea(area);
		}
		//---------------------------------------------------------------------------
		bool Direct3D10ViewportTarget::IsImageryCache() const
		{
			return false;
		}
		//---------------------------------------------------------------------------
	}
}
