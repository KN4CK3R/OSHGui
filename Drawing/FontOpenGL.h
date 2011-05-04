#ifndef __DRAWING_FONTOPENGL_H__
#define __DRAWING_FONTOPENGL_H__

#include "include.h"
#include <gl\GL.h>
#include <gl\GLU.h>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")

#include "IFont.h"

namespace Drawing
{
	class FontOpenGL : public IFont
	{
	public:
		FontOpenGL();
		virtual ~FontOpenGL();

		GLuint GetFont();

		virtual bool Create(LPCSTR fontName, int size, bool bold, bool italic);

	protected:
		GLuint fontID;
	};
}

#endif