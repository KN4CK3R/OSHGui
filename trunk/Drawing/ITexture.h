#ifndef __OSHGUI_DRAWING_ITEXTURE_H__
#define __OSHGUI_DRAWING_ITEXTURE_H__

#include "../Misc/Strings.h"
#include "Color.h"
#include "Point.h"
#include "Size.h"
#include "Rectangle.h"

namespace OSHGui
{
	namespace Drawing
	{
		class ITexture
		{
		public:
			const Size GetSize()
			{
				return size;
			}
			virtual bool IsLocked() = 0;

			virtual bool Create(const Size &size) = 0;
			virtual bool LoadFromFile(const Misc::UnicodeString &filename, const Size &size) = 0;
		
			virtual void BeginUpdate() = 0;
			virtual void EndUpdate() = 0;
			
			virtual void Clear() = 0;
			virtual void Clear(int x, int y, int w, int h) = 0;

			virtual void Fill(Color color) = 0;
			virtual void Fill(int x, int y, int w, int h, Color color) = 0;

			virtual void FillGradient(Color from, Color to, bool updown = true) = 0;
			virtual void FillGradient(Rectangle &rect, Color from, Color to, bool updown = true) = 0;
			virtual void FillGradient(int x, int y, int w, int h, Color from, Color to, bool updown = true) = 0;
			
			virtual void Rotate(int degrees) = 0;

			virtual void Insert(int x, int y, ITexture *texture) = 0;
		
		protected:
			Size size;
		};
	}
}

#endif