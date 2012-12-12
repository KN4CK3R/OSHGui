/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EVENT_MOUSEBUTTON_HPP
#define OSHGUI_EVENT_MOUSEBUTTON_HPP

namespace OSHGui
{
	/**
	 * Gibt Konstanten an, die definieren, welche Maustaste gedrückt wurde.
	 */
	namespace MouseButton
	{
		enum MouseButtons
		{
			None = 0,
			Left = 2,
			Right = 4,
			Middle = 8,
			XButton1 = 16,
			XButton2 = 32,
		};
	}
}

#endif