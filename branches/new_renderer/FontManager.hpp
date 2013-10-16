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
#include "Misc/RawDataContainer.hpp"

namespace OSHGui
{
	class OSHGUI_EXPORT FontManager
	{
	public:
		static Drawing::FontPtr LoadFont(const Misc::AnsiString &name, float pointSize, bool antiAliased);
		static Drawing::FontPtr LoadFont(const Misc::AnsiString &name, float pointSize, bool antiAliased, Drawing::AutoScaleMode autoScaleMode);
		static Drawing::FontPtr LoadFontFromFile(const Misc::AnsiString &filename, float pointSize, bool antiAliased);
		static Drawing::FontPtr LoadFontFromFile(const Misc::AnsiString &filename, float pointSize, bool antiAliased, Drawing::AutoScaleMode autoScaleMode);
		static Drawing::FontPtr LoadFontFromMemory(const Misc::RawDataContainer &data, float pointSize, bool antiAliased);
		static Drawing::FontPtr LoadFontFromMemory(const Misc::RawDataContainer &data, float pointSize, bool antiAliased, Drawing::AutoScaleMode autoScaleMode);

	private:
		static Drawing::FontPtr defaultFont;
	};
}

#endif