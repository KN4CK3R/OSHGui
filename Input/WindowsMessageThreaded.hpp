/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_INPUT_WINDOWSMESSAGETRHEADED_HPP
#define OSHGUI_INPUT_WINDOWSMESSAGETRHEADED_HPP

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include "../Exports.hpp"
#include "WindowsMessage.hpp"
#include "../Misc/ConcurrendBoundedBuffer.hpp"
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
			virtual bool InjectMouseMessage(MouseMessage &&mouse) override;
			virtual bool InjectKeyboardMessage(KeyboardMessage &&keyboard) override;

		private:
			typedef Misc::ConcurrendQueue<MouseMessage> MouseMessageQueue;
			MouseMessageQueue mouseMessages;
			typedef Misc::ConcurrendQueue<KeyboardMessage> KeyboardMessageQueue;
			KeyboardMessageQueue keyboardMessages;
		};
	}
}

#endif