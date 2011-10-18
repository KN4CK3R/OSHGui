#ifndef OSHGUI_DRAWING_FONTOPENGL_H_
#define OSHGUI_DRAWING_FONTOPENGL_H_

#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")

#include "IFont.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class FontOpenGL : public IFont
		{
		public:
			FontOpenGL();
			virtual ~FontOpenGL();

			GLuint GetFont();

			virtual void Create(const Misc::UnicodeString &fontName, int size, bool bold, bool italic);

			virtual const Size MeasureText(const Misc::UnicodeString &str);

		protected:
			GLuint fontID;
		};
	}
}

#endif