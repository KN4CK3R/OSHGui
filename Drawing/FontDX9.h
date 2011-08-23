#ifndef __OSHGUI_DRAWING_FONTDX9_H__
#define __OSHGUI_DRAWING_FONTDX9_H__

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
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

			virtual bool Create(const Misc::UnicodeString &fontName, int size, bool bold, bool italic);
			
			virtual Size MeasureText(const Misc::UnicodeString &str);

		protected:
			IDirect3DDevice9 *device;
			LPD3DXFONT font;
			
			int spaceWidth;
		};
	}
}

#endif