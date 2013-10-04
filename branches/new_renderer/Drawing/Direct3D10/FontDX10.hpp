/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_FONTDX10_HPP
#define OSHGUI_DRAWING_FONTDX10_HPP

#include <d3dx10.h>

#include "IFont.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class FontDX10 : public IFont
		{
		public:
			FontDX10(ID3D10Device *device);
			~FontDX10();

			LPD3DX10FONT GetFont();

			virtual bool Create(const Misc::UnicodeString &fontName, int size, bool bold, bool italic);
			
		private:
			ID3D10Device *device;
			LPD3DX10FONT font;
		};
	}
}

#endif