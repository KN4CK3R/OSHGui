/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EVENT_KEY_HPP
#define OSHGUI_EVENT_KEY_HPP

#include "../Exports.hpp"

namespace OSHGui
{
	/**
	 * Stellt Tastencodes bereit.
	 */
	class OSHGUI_EXPORT Key
	{
	public:
		/**
		 * Aufzählung der Tastencodes
		 */
		enum Keys
		{
			None,
			D0, D1, D2, D3, D4, D5, D6, D7, D8, D9,
			A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			Space, Back,
			Insert, Home, End, Delete, PageUp, PageDown,
			Alt, Control, Shift, Tab,
			Return, Escape,
			Up, Right, Down, Left,
			Divide, Multiply, Substract, Add,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
		};
	};
 
	inline Key::Keys operator | (const Key::Keys a, const Key::Keys b)
	{
		return Key::Keys(int(a) | int(b));
	}

	inline Key::Keys& operator |= (Key::Keys& a, const Key::Keys b)
	{
		a = a | b;
		return a;
	}

	inline Key::Keys operator & (const Key::Keys a, const Key::Keys b)
	{
		return Key::Keys(int(a) & int(b));
	}

	inline Key::Keys& operator &= (Key::Keys& a, const Key::Keys b)
	{
		a = a & b;
		return a;
	}
}

#endif