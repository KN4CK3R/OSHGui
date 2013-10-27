/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_INPUT_WINDOWSMESSAGE_HPP
#define OSHGUI_INPUT_WINDOWSMESSAGE_HPP

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include "../Exports.hpp"


namespace OSHGui
{
	class MouseMessage;
	class KeyboardMessage;

	namespace Input
	{
		/**
		 * Verwaltet den Input unter Windows.
		 */
		class OSHGUI_EXPORT WindowsMessage
		{
		public:
			WindowsMessage();
			virtual ~WindowsMessage();

			/**
			 * Wandelt eine Windows Message in ein Event um.
			 *
			 * \param message
			 * \return
			 */
			bool ProcessMessage(LPMSG message);

		protected:
			virtual bool InjectMouseMessage(MouseMessage &mouse);
			virtual bool InjectKeyboardMessage(KeyboardMessage &keyboard);

		private:
			static const int SystemDefaultCharSize = 2;

			int ImeWmCharsToIgnore;
		};
	}
}

#endif