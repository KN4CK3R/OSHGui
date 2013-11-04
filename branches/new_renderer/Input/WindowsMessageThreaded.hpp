/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_INPUT_WINDOWSMESSAGETRHEADED_HPP
#define OSHGUI_INPUT_WINDOWSMESSAGETRHEADED_HPP

#include "WindowsMessage.hpp"
#include "../Event/MouseMessage.hpp"
#include "../Event/KeyboardMessage.hpp"

namespace OSHGui
{
	namespace Input
	{
		/**
		 * Verwaltet den Input unter Windows und gibt diesen threadsafe an die Application weiter.
		 */
		class OSHGUI_EXPORT WindowsMessageThreaded : public WindowsMessage
		{
		public:
			void PopulateMessages();

		protected:
			virtual bool InjectMouseMessage(MouseMessage &mouse) override;
			virtual bool InjectKeyboardMessage(KeyboardMessage &keyboard) override;

		private:
			MouseMessageQueue mouseMessages;
			KeyboardMessageQueue keyboardMessages;
		};
	}
}

#endif