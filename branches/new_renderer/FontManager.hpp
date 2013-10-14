/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_FONTMANAGER_HPP
#define OSHGUI_FONTMANAGER_HPP

#include <memory>
#include "Exports.hpp"
#include "Drawing/Font.hpp"

namespace OSHGui
{
	class OSHGUI_EXPORT FontManager
	{
	public:
		Drawing::FontPtr& GetDefaultFont();
		void SetDefaultFont(Drawing::FontPtr &defaultFont);

		Drawing::FontPtr LoadFont(const Misc::AnsiString &name, float pointSize, bool antiAliased);
		Drawing::FontPtr LoadFont(const Misc::AnsiString &name, float pointSize, bool antiAliased, Drawing::AutoScaleMode autoScaleMode);
		Drawing::FontPtr LoadFontFromFile(const Misc::AnsiString &filename, float pointSize, bool antiAliased);
		Drawing::FontPtr LoadFontFromFile(const Misc::AnsiString &filename, float pointSize, bool antiAliased, Drawing::AutoScaleMode autoScaleMode);

	private:
		Drawing::FontPtr defaultFont;
	};
}

#endif