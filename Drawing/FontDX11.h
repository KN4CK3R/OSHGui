#ifndef __OSHGUI_DRAWING_FONTDX11_H__
#define __OSHGUI_DRAWING_FONTDX11_H__

#include <d3dx11.h>

#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\IFont.h"

namespace OSHGui
{
	namespace Drawing
	{
		class FontDX11 : public IFont
		{
		public:
			FontDX11(ID3D11Device *device);
			~FontDX11();

			LPD3DX11FONT GetFont();

			virtual bool Create(LPCWSTR fontName, int size, bool bold, bool italic);
			
		private:
			ID3D11Device *device;
			LPD3DX11FONT font;
		};
	}
}

#endif