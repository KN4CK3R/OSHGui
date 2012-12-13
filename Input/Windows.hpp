/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_INPUT_WINDOWS_HPP
#define OSHGUI_INPUT_WINDOWS_HPP

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include "../Exports.hpp"

namespace OSHGui
{
	namespace Input
	{
		/**
		 * Verwaltet den Input unter Windows.
		 */
		class OSHGUI_EXPORT Windows
		{
		public:
			Windows();

			/**
			 * Wandelt eine Windows Message in ein Event um.
			 *
			 * @param message
			 * @return
			 */
			bool ProcessMessage(LPMSG message);

		private:
			static const int SystemDefaultCharSize = 2;

			int ImeWmCharsToIgnore;
		};
	}
}

#endif