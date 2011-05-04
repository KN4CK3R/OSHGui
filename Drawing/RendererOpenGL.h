#ifndef __DRAWING_RENDEREROPENGL_H__
#define __DRAWING_RENDEREROPENGL_H__

#include "include.h"
#include <gl\GL.h>
#include <gl\GLU.h>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")

#include "IRenderer.h"
#include "FontOpenGL.h"

namespace Drawing
{	
	class RendererOpenGL : public IRenderer
	{
	public:
		RendererOpenGL();

		virtual void Begin();
		virtual void End();

		virtual Drawing::IFont* CreateNewFont();

		virtual void SetRenderColor(Drawing::Color color);
	
		//virtual Drawing::Size MeasureText(Drawing::IFont *font, LPCSTR text) = 0;
		//virtual Drawing::Size MeasureTextEx(Drawing::IFont *font, LPCSTR text, ...) = 0;

		virtual void RenderText(Drawing::IFont *font, const Drawing::Point &point, LPCSTR text);
		virtual void RenderText(Drawing::IFont *font, int x, int y, LPCSTR text);
		virtual void RenderText(Drawing::IFont *font, Drawing::Rectangle &rect, LPCSTR text);
		virtual void RenderText(Drawing::IFont *font, int x, int y, int w, int h, LPCSTR text);
		
		virtual void RenderTextEx(Drawing::IFont *font, const Drawing::Point &point, LPCSTR text, ...);
		virtual void RenderTextEx(Drawing::IFont *font, int x, int y, LPCSTR text, ...);
		virtual void RenderTextEx(Drawing::IFont *font, Drawing::Rectangle &rect, LPCSTR text, ...);
		virtual void RenderTextEx(Drawing::IFont *font, int x, int y, int w, int h, LPCSTR text, ...);
			
		virtual void Fill(const Drawing::Point &point);
		virtual void Fill(int x, int y);
		virtual void Fill(Drawing::Rectangle &rect);
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

#endif