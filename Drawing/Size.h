#ifndef __OSHGUI_DRAWING_SIZE_H__
#define __OSHGUI_DRAWING_SIZE_H__

namespace OSHGui
{
	namespace Drawing
	{
		class Size
		{			
		public:
			Size();
			Size(int Width, int Height);
			Size(const Size &s);
			
			bool operator == (const Size &equal);
			bool operator != (const Size &equal);
			Size operator - (const Size &s);
			void operator -= (const Size &s);
			Size operator + (const Size &s);
			void operator += (const Size &s);
			
			void Inflate(int Width, int Height);
			Size InflateEx(int Width, int Height);
			
			bool Contains(const Point &point);
			
			int Width,
				Height;
		};
	}
}

#endif