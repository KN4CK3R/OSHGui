#ifndef OSHGUI_DRAWING_VERTEX_HPP
#define OSHGUI_DRAWING_VERTEX_HPP

#include "Vector.hpp"
#include "Color.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class OSHGUI_EXPORT Vertex
		{
		public:
			//! Position of the vertex in 3D space.
			Vector Position;
			//! colour to be applied to the vertex.
			Drawing::Color Color;
			//! Texture co-ords to be applied to the vertex.
			PointF TextureCoordinates;
		};
	}
}

#endif
