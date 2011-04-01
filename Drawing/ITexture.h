#ifndef __OSHGUI_DRAWING_ITEXTURE_H__
#define __OSHGUI_DRAWING_ITEXTURE_H__

#include <math.h>

#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\Point.h"
#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\Size.h"
#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\Rectangle.h"

#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = NULL; } }
#endif
#ifndef ALPHA
	#define ALPHA(COLORCODE)((int)(COLORCODE >> 24))
#endif
#ifndef RED
	#define RED(COLORCODE)((int) (COLORCODE >> 16) & 0xFF)
#endif
#ifndef GREEN
	#define GREEN(COLORCODE)((int)(COLORCODE >> 8) & 0xFF)
#endif
#ifndef BLUE
	#define BLUE(COLORCODE)((int) COLORCODE & 0xFF)
#endif
#ifndef _max
	#define _max(a, b) (a > b ? a : b)
#endif
#ifndef _min
	#define _min(a, b) (a < b ? a : b)
#endif
#ifndef D3DCOLOR
	#define D3DCOLOR DWORD
#endif

namespace OSHGui
{
	namespace Drawing
	{
		class ITexture
		{
		public:		
			const Drawing::Size GetSize()
			{
				return size;
			}
			virtual bool IsLocked() = 0;

			virtual bool Create(const Drawing::Size &size) = 0;
		
			virtual void BeginUpdate() = 0;
			virtual void EndUpdate() = 0;
			
			virtual void Clear() = 0;
			virtual void Clear(int x, int y, int w, int h) = 0;

			virtual void Fill(D3DCOLOR color) = 0;
			virtual void Fill(int x, int y, int w, int h, D3DCOLOR color) = 0;

			virtual void FillGradient(int x, int y, int w, int h, D3DCOLOR from, D3DCOLOR to, bool updown = true) = 0;
			
			virtual void Rotate(int degrees) = 0;

			virtual void Insert(int x, int y, Drawing::ITexture *texture) = 0;
		
		protected:
			Drawing::Size size;
		};
	}
}

#endif