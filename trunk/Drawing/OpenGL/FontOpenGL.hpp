/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_FONTOPENGL_HPP
#define OSHGUI_DRAWING_FONTOPENGL_HPP

#include "../IFont.hpp"
#define NOMINMAX
#include <windows.h>

namespace OSHGui
{
	namespace Drawing
	{
		class FontOpenGL : public IFont
		{
		public:
			FontOpenGL(const Misc::AnsiString &name, int size, bool bold, bool italic);
			virtual ~FontOpenGL();

			unsigned int GetFont();

			virtual const Size MeasureText(const Misc::AnsiString &str);

		private:
			virtual void Create(const Misc::AnsiString &name, int size, bool bold, bool italic);

			unsigned int fontID;
			HFONT font;
		};
	}
}

#endif