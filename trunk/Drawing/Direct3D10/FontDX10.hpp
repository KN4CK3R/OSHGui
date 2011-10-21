#ifndef OSHGUI_DRAWING_FONTDX10_HPP_
#define OSHGUI_DRAWING_FONTDX10_HPP_

#include <d3dx10.h>

#include "IFont.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class FontDX10 : public IFont
		{
		public:
			FontDX10(ID3D10Device *device);
			~FontDX10();

			LPD3DX10FONT GetFont();

			virtual bool Create(const Misc::UnicodeString &fontName, int size, bool bold, bool italic);
			
		private:
			ID3D10Device *device;
			LPD3DX10FONT font;
		};
	}
}

#endif