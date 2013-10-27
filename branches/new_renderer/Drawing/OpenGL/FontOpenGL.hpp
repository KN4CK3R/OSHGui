/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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

			virtual const SizeF MeasureText(const Misc::AnsiString &str);

		private:
			unsigned int fontID;
			HFONT font;
		};
	}
}

#endif