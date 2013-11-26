/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "WindowsMessageThreaded.hpp"
#include "../Application.hpp"

namespace OSHGui
{
	namespace Input
	{
		void WindowsMessageThreaded::PopulateMessages()
		{
			auto app = Application::Instance();

			while (!mouseMessages.empty())
			{
				app->ProcessMouseMessage(mouseMessages.Get());
			}
			while (!keyboardMessages.empty())
			{
				app->ProcessKeyboardMessage(keyboardMessages.Get());
			}
		}
		//---------------------------------------------------------------------------
		bool WindowsMessageThreaded::InjectMouseMessage(MouseMessage &mouse)
		{
			mouseMessages.Push(mouse);

			return false;
		}
		//---------------------------------------------------------------------------
		bool WindowsMessageThreaded::InjectKeyboardMessage(KeyboardMessage &keyboard)
		{
			keyboardMessages.Push(keyboard);

			return false;
		}
		//---------------------------------------------------------------------------
	}
}