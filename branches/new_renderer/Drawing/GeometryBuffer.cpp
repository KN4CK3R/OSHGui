#include "GeometryBuffer.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		GeometryBuffer::GeometryBuffer()
			: drawMode(VertexDrawMode::Triangle)
		{
		}
		//---------------------------------------------------------------------------
		GeometryBuffer::~GeometryBuffer()
		{
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		void GeometryBuffer::SetVertexDrawMode(VertexDrawMode mode)
		{
			drawMode = mode;
		}
		//---------------------------------------------------------------------------
	}
}
