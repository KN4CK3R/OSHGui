#ifndef __OSHGUI_DRAWING_FONTDX9_H__
#define __OSHGUI_DRAWING_FONTDX9_H__

#include <d3dx9.h>

#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\IFont.h"

namespace OSHGui
{
	namespace Drawing
	{
		class FontDX9 : public IFont
		{
		public:
			FontDX9(IDirect3DDevice9 *device);
			~FontDX9();

			LPD3DXFONT GetFont();

			virtual bool Create(LPCSTR fontName, int size, bool bold, bool italic);
			
		private:
			IDirect3DDevice9 *device;
			LPD3DXFONT font;
		};
	}
}

#endif