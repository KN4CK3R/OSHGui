/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "TextureOpenGL.hpp"
#define NOMINMAX
#include "../../Misc/Exceptions.hpp"
#include <fstream>
#include <gl/glut.h>

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		TextureOpenGL::TextureOpenGL(const SizeF &size, int frameCount, Misc::TimeSpan frameChangeInterval)
			: frameBuffer(0)
		{
			Create(size, frameCount);
		}
		//---------------------------------------------------------------------------
		TextureOpenGL::TextureOpenGL(const Misc::AnsiString &filename)
			: frameBuffer(0)
		{

		}
		//---------------------------------------------------------------------------
		TextureOpenGL::~TextureOpenGL()
		{
			if (IsLocked())
			{
				EndUpdate();
			}
			for (auto it = std::begin(frames); it != std::end(frames); ++it)
			{
				glDeleteTextures(1, &*it);
			}
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		unsigned int TextureOpenGL::GetTexture() const
		{
			return texture;
		}
		//---------------------------------------------------------------------------
		bool TextureOpenGL::IsLocked() const
		{
			return false;// frameBuffer != 0;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void TextureOpenGL::Create(const SizeF &size, int frameCount)
		{
			for (int i = 0; i < frameCount; ++i)
			{
				unsigned int texture;
				glGenTextures(1, &texture);
				if (glGetError() != GL_NO_ERROR)
				{
					#ifndef OSHGUI_DONTUSEEXCEPTIONS
					throw Misc::Exception("Cannot create Texture.", __FILE__, __LINE__);
					#else
					throw 1;
					#endif
				}

				glBindTexture(GL_TEXTURE_2D, texture);

				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.Width, size.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

				frames.push_back(texture);
			}

			this->size = size;

			SelectActiveFrame(0);
		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::LoadFromFile(const Misc::AnsiString &filename)
		{

		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::BeginUpdate()
		{
// 			glGenFramebuffers(1, frameBuffer);
// 			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
// 
// 			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
// 			GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0 };
// 			glDrawBuffers(1, drawBuffers);
// 
// 			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
// 			{
// 				#ifndef OSHGUI_DONTUSEEXCEPTIONS
// 				throw Misc::Exception("Cannot begin update on texture.", __FILE__, __LINE__);
// 				#else
// 				throw 1;
// 				#endif
// 			}
// 
// 			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
			glViewport(0, 0, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::EndUpdate()
		{
			glFlush();

			glBindTexture(GL_TEXTURE_2D, texture);
			glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, size.Width, size.Height, 0);

			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//glDeleteFramebuffers(1, frameBuffer);
		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::Fill(int x, int y, int w, int h, Color color)
		{
			glColor4ub(color.R, color.G, color.B, color.A);
			glBegin(GL_QUADS);
			{
				glVertex2f(x, y);
				glVertex2f(x, y + h);
				glVertex2f(x + w, y + h);
				glVertex2f(x + w, y);
			}
			glEnd();
		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::FillGradient(int x, int y, int w, int h, Color from, Color to, bool updown)
		{
			glBegin(GL_QUADS);
			{
				glColor4ub(from.R, from.G, from.B, from.A);
				glVertex2f(x, y);
				if (updown)
				{

					glVertex2f(x + w, y);
					glColor4ub(to.R, to.G, to.B, to.A);
					glVertex2f(x, y + h);
				}
				else
				{
					glVertex2f(x, y + h);
					glColor4ub(to.R, to.G, to.B, to.A);
					glVertex2f(x + w, y);
				}
				glVertex2f(x + w, y + h);
			}
			glEnd();
		}
		//---------------------------------------------------------------------------
		int TextureOpenGL::GetFrameCount() const
		{
			return frames.size();
		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::SelectActiveFrame(int frame)
		{
			if (IsLocked())
			{
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw Misc::Exception("Cannot select frame while texture is locked.", __FILE__, __LINE__);
				#else
				throw 1;
				#endif
			}

			if (frame < 0 || frame >= GetFrameCount())
			{
				frame = 0;
			}

			this->frame = frame;

			texture = frames[this->frame];
		}
		//---------------------------------------------------------------------------
	}
}