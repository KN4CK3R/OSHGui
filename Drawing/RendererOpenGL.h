#ifndef __OSHGUI_DRAWING_RENDEREROPENGL_H__
#define __OSHGUI_DRAWING_RENDEREROPENGL_H__

#include <windows.h>
#include <stdio.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")

#include "IRenderer.h"
#include "FontOpenGL.h"

namespace OSHGui
{
	namespace Drawing
	{
		class RendererOpenGL : public IRenderer
		{
		public:
			RendererOpenGL();

			virtual void Begin();
			virtual void End();

			virtual IFont* CreateNewFont();

			virtual void SetRenderColor(Color color);
	
			//virtual Size MeasureText(IFont *font, const Misc::UnicodeString &text) = 0;

			virtual void RenderText(IFont *font, const Point &point, const Misc::UnicodeString &text);
			virtual void RenderText(IFont *font, int x, int y, const Misc::UnicodeString &text);
			virtual void RenderText(IFont *font, Rectangle &rect, const Misc::UnicodeString &text);
			virtual void RenderText(IFont *font, int x, int y, int w, int h, const Misc::UnicodeString &text);
			
			virtual void Fill(const Point &point);
			virtual void Fill(int x, int y);
			virtual void Fill(Rectangle &rect);
			virtual void Fill(int x, int y, int w, int h);

			struct
			{
				int x;
				int y;
				int width;
				int height;
			} glViewport;

		protected:
			void Flush();
			void AddVertex(int x, int y);

			struct glLoc
			{
				float x, y, z/*, w*/;
			};
			struct glUV
			{
				float u, v;
			};

			static const int maxVertices = 1024;
			glLoc verticesLoc[maxVertices];
			DWORD verticesColor[maxVertices];

			int verticesNum;

			ITexture *texture;
		};
	}
}

#endif