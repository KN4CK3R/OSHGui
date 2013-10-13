#include "GeometryBuffer.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		GeometryBuffer::GeometryBuffer()
			: blendMode(BlendMode::Normal)
		{
		}
		//---------------------------------------------------------------------------
		GeometryBuffer::~GeometryBuffer()
		{
		}
		//---------------------------------------------------------------------------
		void GeometryBuffer::SetBlendMode(const BlendMode mode)
		{
			blendMode = mode;
		}
		//---------------------------------------------------------------------------
		BlendMode GeometryBuffer::GetBlendMode() const
		{
			return blendMode;
		}
		//---------------------------------------------------------------------------
	}
}
