#ifndef __OSHGUI_DRAWING_FONTOPENGL_H__
#define __OSHGUI_DRAWING_FONTOPENGL_H__

#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")

#include "IFont.h"

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

			virtual bool Create(const Misc::UnicodeString &fontName, int size, bool bold, bool italic);

			virtual Size MeasureText(const Misc::UnicodeString &str);

		protected:
			GLuint fontID;
		};
	}
}

#endif