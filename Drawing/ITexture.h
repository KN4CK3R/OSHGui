#ifndef __OSHGUI_DRAWING_ITEXTURE_H__
#define __OSHGUI_DRAWING_ITEXTURE_H__

#include <math.h>

#include "E:\Coding\OSHGui\Drawing\Color.h"
#include "E:\Coding\OSHGui\Drawing\Point.h"
#include "E:\Coding\OSHGui\Drawing\Size.h"
#include "E:\Coding\OSHGui\Drawing\Rectangle.h"

#ifndef _max
	#define _max(a, b) (a > b ? a : b)
#endif
#ifndef _min
	#define _min(a, b) (a < b ? a : b)
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

			virtual void Fill(Drawing::Color color) = 0;
			virtual void Fill(int x, int y, int w, int h, Drawing::Color color) = 0;

			virtual void FillGradient(Drawing::Color from, Drawing::Color to, bool updown = true) = 0;
			virtual void FillGradient(Drawing::Rectangle &rect, Drawing::Color from, Drawing::Color to, bool updown = true) = 0;
			virtual void FillGradient(int x, int y, int w, int h, Drawing::Color from, Drawing::Color to, bool updown = true) = 0;
			
			virtual void Rotate(int degrees) = 0;

			virtual void Insert(int x, int y, Drawing::ITexture *texture) = 0;
		
		protected:
			Drawing::Size size;
		};
	}
}

#endif