#ifndef __OSHGUI_DRAWING_SIZE_H__
#define __OSHGUI_DRAWING_SIZE_H__

namespace OSHGui
{
	namespace Drawing
	{
		class Size
		{
		public:
			int Width,
				Height;
			
		public:
			Size() { Size(0, 0); }
			Size(int Width, int Height) { this->Width = Width; this->Height = Height; }
			Size(const Size &s) { Width = s.Width; Height = s.Height; }
			
			bool operator == (const Size &equal) { return Width == equal.Width && Height == equal.Height; }
			const bool operator == (const Size &equal) { return Width == equal.Width && Height == equal.Height; }
			bool operator != (const Size &equal) { return Width != equal.Width || Height != equal.Height; }
			const bool operator != (const Size &equal) { return Width != equal.Width || Height != equal.Height; }
			
			void Inflate(int Width, int Height)
			{
				this->Width += Width;
				this->Height += Height;
			}
			
			Size InflateEx(int Width, int Height)
			{
				return Size(this->Width + Width, this->Height + Height);
			}
		};
	}
}

#endif