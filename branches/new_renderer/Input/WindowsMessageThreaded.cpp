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

			while (!mouseMessages.IsEmpty())
			{
				app->ProcessMouseMessage(mouseMessages.Pop());
			}
			while (!keyboardMessages.IsEmpty())
			{
				app->ProcessKeyboardMessage(keyboardMessages.Pop());
			}
		}
		//---------------------------------------------------------------------------
		bool WindowsMessageThreaded::InjectMouseMessage(MouseMessage &&mouse)
		{
			mouseMessages.Push(mouse);

			return false;
		}
		//---------------------------------------------------------------------------
		bool WindowsMessageThreaded::InjectKeyboardMessage(KeyboardMessage &&keyboard)
		{
			keyboardMessages.Push(keyboard);

			return false;
		}
		//---------------------------------------------------------------------------
	}
}