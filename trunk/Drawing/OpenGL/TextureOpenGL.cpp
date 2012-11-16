/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "TextureOpenGL.hpp"
#define NOMINMAX
#include "../../Misc/Exceptions.hpp"
#include <fstream>

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		TextureOpenGL::TextureOpenGL(const Size &size, int frameCount, Misc::TimeSpan frameChangeInterval)
		{

		}
		//---------------------------------------------------------------------------
		TextureOpenGL::TextureOpenGL(int width, int height, int frameCount, Misc::TimeSpan frameChangeInterval)
		{

		}
		//---------------------------------------------------------------------------
		TextureOpenGL::TextureOpenGL(const Misc::AnsiString &filename)
		{

		}
		//---------------------------------------------------------------------------
		TextureOpenGL::~TextureOpenGL()
		{

		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		bool TextureOpenGL::IsLocked() const
		{
			return false;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void TextureOpenGL::Create(const Size &size, int frameCount)
		{

		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::LoadFromFile(const Misc::AnsiString &filename)
		{

		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::BeginUpdate()
		{

		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::EndUpdate()
		{

		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::Fill(int x, int y, int w, int h, Color _color)
		{		

		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::FillGradient(int j, int i, int w, int h, Color from, Color to, bool updown)
		{

		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::Rotate(int degrees)
		{

		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::Insert(int j, int i, const std::shared_ptr<ITexture> &texture)
		{

		}
		//---------------------------------------------------------------------------
		int TextureOpenGL::GetFrameCount() const
		{
			return 0;
		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::AddFrame(const std::shared_ptr<ITexture> &frame)
		{

		}
		//---------------------------------------------------------------------------
		void TextureOpenGL::SelectActiveFrame(int frame)
		{

		}
		//---------------------------------------------------------------------------
	}
}