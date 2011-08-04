#include "RendererOpenGL.h"

namespace OSHGui
{
	namespace Drawing
	{
		RendererOpenGL::RendererOpenGL()
		{
			for (int i = 0; i < maxVertices; i++)
			{
				verticesLoc[i].z = 0.5f;
				//verticesLoc[i].w = 1.0f;
			}
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::Begin()
		{
			glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_SCISSOR_BIT | GL_TRANSFORM_BIT);

			glDisable(GL_LIGHTING);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 

			glGetIntegerv(GL_VIEWPORT, &glViewport.x);

			glDisable(GL_DEPTH_TEST);
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, glViewport.width, 0, glViewport.height, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glTranslated(0, 0, 0);

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);

			glNormal3f(0.0f, 0.0f, 1.0f);
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::End()
		{
			glPopMatrix();										
			glMatrixMode(GL_PROJECTION);						
			glPopMatrix();
			glPopAttrib();
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::SetRenderColor(Color color)
		{
			IRenderer::SetRenderColor(color);
			glColor4f(color.R, color.G, color.B, color.A);
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::Flush()
		{
			if (verticesNum > 0)
			{
				glVertexPointer(3, GL_FLOAT,  sizeof(glLoc), (const GLvoid *)verticesLoc);
				glEnableClientState(GL_VERTEX_ARRAY);
				glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(DWORD), (const GLvoid*)verticesColor);
				glEnableClientState(GL_COLOR_ARRAY);
				//glTexCoordPointer(2, GL_FLOAT, sizeof(glUV), (const GLvoid *)m_pVertsUV);
				//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glDrawArrays(GL_TRIANGLES, 0, (GLsizei) verticesNum);

				verticesNum = 0;
				glFlush();
			}
		}
		//---------------------------------------------------------------------------
		IFont* RendererOpenGL::CreateNewFont()
		{
			FontOpenGL *newFont = new FontOpenGL();
			return newFont;
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::RenderText(IFont *font, const Point &point, const Misc::UnicodeString &text)
		{
			RenderText(font, point.X, point.Y, 0, 0, text);
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::RenderText(IFont *font, int x, int y, const Misc::UnicodeString &text)
		{
			RenderText(font, x, y, 0, 0, text);
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::RenderText(IFont *font, Rectangle &rectangle, const Misc::UnicodeString &text)
		{
			RenderText(font, rectangle.GetLeft(), rectangle.GetTop(), rectangle.GetWidth(), rectangle.GetHeight(), text);
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::RenderText(IFont *font, int x, int y, int w, int h, const Misc::UnicodeString &text)
		{
			if (font == NULL)
			{
				return;
			}

			GLuint fontID = ((FontOpenGL*)font)->GetFont();
			if (fontID == -1)
			{
				return;
			}

			glPushAttrib(GL_LIST_BIT);
			glRasterPos2f(x, glViewport.height - y);
			glListBase(fontID);
			glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());
			glPopAttrib();
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::Fill(const Point &point)
		{
			Fill(point.X, point.Y, 1, 1);
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::Fill(int x, int y)
		{
			Fill(x, y, 1, 1);
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::Fill(Rectangle &rect)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::Fill(int x, int y, int w, int h)
		{
			y = glViewport.height - y;

			glBegin(GL_QUADS);
			{
				glVertex2f(x, y); 
				glVertex2f(x, y - h); 
				glVertex2f(x + w, y - h); 
				glVertex2f(x + w, y);
			}
			glEnd();
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::AddVertex(int x, int y)
		{
			if (verticesNum >= maxVertices - 1)
			{
				Flush();
			}

			verticesLoc[verticesNum].x = (float)x;
			verticesLoc[verticesNum].y = (float)y;
			verticesColor[verticesNum] = color.ARGB;

			verticesNum++;
		}
		//---------------------------------------------------------------------------
	}
}