#ifndef __OSHGUI_DRAWING_FONTDX9_H__
#define __OSHGUI_DRAWING_FONTDX9_H__

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "IFont.h"

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
			
			virtual int MeasureCharacter(char c);

		protected:
			IDirect3DDevice9 *device;
			LPD3DXFONT font;
			
			bool initWidthArray;
			int charWidth[0xFF];
		};
	}
}

#endif