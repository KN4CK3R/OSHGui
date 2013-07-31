/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include <windows.h>
#include <gl/glut.h>
#include "RendererOpenGL.hpp"
#include "FontOpenGL.hpp"
#include "TextureOpenGL.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		RendererOpenGL::RendererOpenGL()
			: verticesNum(0),
			  texture(0)
		{
			for (int i = 0; i < maxVertices; ++i)
			{
				vertices[i].z = 0.0f;
				verticesUV[i].u = verticesUV[i].v = 0.0f;
			}

			defaultFont = CreateNewFont("Arial", 14, false, false);
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		const Size RendererOpenGL::GetRenderDimension() const
		{
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			return Size(viewport[2], viewport[3]);
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::Begin()
		{
			glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_SCISSOR_BIT | GL_TRANSFORM_BIT);

			glDisable(GL_LIGHTING);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 

			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);

			glDisable(GL_DEPTH_TEST);
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, viewport[2], viewport[3], 0, 0, 1);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glTranslated(0, 0, 0);

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			//glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
			glDisableClientState(GL_INDEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			//glDisableClientState(GL_FOG_COORDINATE_ARRAY);
			glDisableClientState(GL_EDGE_FLAG_ARRAY);

			glNormal3f(0.0f, 0.0f, 1.0f);
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::End()
		{
			Flush();

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			glPopAttrib();
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::SetRenderColor(Color color)
		{
			glColor4ub(color.R, color.G, color.B, color.A);
			std::swap(color.R, color.B);
			IRenderer::SetRenderColor(color);
		}
		//---------------------------------------------------------------------------
		Color RendererOpenGL::GetRenderColor() const
		{
			Color color = IRenderer::GetRenderColor();
			std::swap(color.R, color.B);
			return color;
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::Flush()
		{
			if (verticesNum > 0)
			{
				glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertices);
				glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(DWORD), verticesColor);
				glTexCoordPointer(2, GL_FLOAT, sizeof(VertexUV), verticesUV);
				glDrawArrays(GL_TRIANGLES, 0, verticesNum);

				verticesNum = 0;

				glFlush();
			}
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<ITexture> RendererOpenGL::CreateNewTexture(const Size &size, int frameCount, Misc::TimeSpan frameChangeInterval)
		{
			return std::make_shared<TextureOpenGL>(size, frameCount, frameChangeInterval);
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<ITexture> RendererOpenGL::CreateNewTexture(int width, int height, int frameCount, Misc::TimeSpan frameChangeInterval)
		{
			return std::make_shared<TextureOpenGL>(Size(width, height), frameCount, frameChangeInterval);
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<ITexture> RendererOpenGL::CreateNewTexture(const Misc::AnsiString &filename)
		{
			return std::make_shared<TextureOpenGL>(filename);
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<IFont> RendererOpenGL::CreateNewFont(const Misc::AnsiString &name, int size, bool bold, bool italic)
		{
			std::shared_ptr<FontOpenGL> font(new FontOpenGL(fontName, size, bold, italic));
			//fontList.push_back(std::weak_ptr<FontDX9>(font));
			return font;
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y, int w, int h)
		{
			if (!texture)
			{
				return;
			}
			
			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();

			glBindTexture(GL_TEXTURE_2D, std::static_pointer_cast<TextureOpenGL>(texture)->GetTexture());

			AddVertex(x, y, 0.0f, 0.0f);
			AddVertex(x + w, y, 1.0f, 0.0f);
			AddVertex(x, y + h, 0.0f, 1.0f);
			AddVertex(x + w, y, 1.0f, 0.0f);
			AddVertex(x + w, y + h, 1.0f, 1.0f);
			AddVertex(x, y + h, 0.0f, 1.0f);
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::RenderText(const std::shared_ptr<IFont> &font, int x, int y, int w, int h, const Misc::AnsiString &text)
		{
			if (!font || text.empty())
			{
				return;
			}

			Flush();

			GLuint fontID = std::static_pointer_cast<FontOpenGL>(font)->GetFont();
			if (fontID == -1)
			{
				return;
			}

			glPushAttrib(GL_LIST_BIT);
			glRasterPos2f(x, y + font->GetSize() - 3);
			glListBase(fontID);
			glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());
			glPopAttrib();
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::Fill(int x, int y, int w, int h)
		{
			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();

			AddVertex(x, y);
			AddVertex(x + w, y);
			AddVertex(x, y + h);
			AddVertex(x + w, y);
			AddVertex(x + w, y + h);
			AddVertex(x, y + h);
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::FillGradient(int x, int y, int w, int h, Color to)
		{
			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();

			Color bckColor = GetRenderColor();

			AddVertex(x, y);
			AddVertex(x + w, y);

			SetRenderColor(to);

			AddVertex(x, y + h);

			AddVertex(x + w, y + h);
			AddVertex(x, y + h);

			SetRenderColor(bckColor);

			AddVertex(x + w, y);
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::BeginLines()
		{
			Flush();
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::RenderLine(int x1, int y1, int x2, int y2)
		{
			if (verticesNum >= maxVertices - 2)
			{
				EndLines();
			}

			vertices[verticesNum].x = (float)x1;
			vertices[verticesNum].y = (float)y1;
			verticesColor[verticesNum] = color.ARGB;

			++verticesNum;

			vertices[verticesNum].x = (float)x2;
			vertices[verticesNum].y = (float)y2;
			verticesColor[verticesNum] = color.ARGB;

			++verticesNum;
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::EndLines()
		{
			if (verticesNum > 0)
			{
				glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertices);
				glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(DWORD), verticesColor);
				glDrawArrays(GL_LINES, 0, verticesNum);

				verticesNum = 0;

				glFlush();
			}
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::AddVertex(int x, int y)
		{
			if (verticesNum >= maxVertices - 1)
			{
				Flush();
			}

			vertices[verticesNum].x = (float)x;
			vertices[verticesNum].y = (float)y;

			verticesColor[verticesNum] = color.ARGB;

			++verticesNum;
		}
		//---------------------------------------------------------------------------
		void RendererOpenGL::AddVertex(int x, int y, float u, float v)
		{
			if (verticesNum >= maxVertices - 1)
			{
				Flush();
			}

			vertices[verticesNum].x = (float)x;
			vertices[verticesNum].y = (float)y;

			verticesUV[verticesNum].u = u;
			verticesUV[verticesNum].v = v;

			verticesColor[verticesNum] = color.ARGB;

			++verticesNum;
		}
		//---------------------------------------------------------------------------
	}
}