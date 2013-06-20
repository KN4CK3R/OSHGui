/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "WindowsMessage.hpp"
#include "../Event/MouseMessage.hpp"
#include "../Event/KeyboardMessage.hpp"
#include "../Application.hpp"

namespace OSHGui
{
	namespace Input
	{
		WindowsMessage::WindowsMessage()
			: ImeWmCharsToIgnore(0)
		{

		}

		bool WindowsMessage::ProcessMessage(LPMSG message)
		{
			switch (message->message)
			{
				case WM_MOUSEMOVE:
				case WM_LBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_RBUTTONDOWN:
				case WM_RBUTTONUP:
				case WM_MOUSEWHEEL:
				{
					#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
					#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

					static Drawing::Point lastMouseLocation;

					MouseMessage mouse(MouseMessage::Unknown, MouseButton::None, Drawing::Point(GET_X_LPARAM(message->lParam), GET_Y_LPARAM(message->lParam)), 0);

					switch (message->message)
					{
						case WM_MOUSEMOVE:
							mouse.State = MouseMessage::Move;
							break;
						case WM_LBUTTONDOWN:
							SetCapture(message->hwnd);
							mouse.State = MouseMessage::Down;
							mouse.Button = MouseButton::Left;
							break;
						case WM_LBUTTONUP:
							ReleaseCapture();
							mouse.State = MouseMessage::Up;
							mouse.Button = MouseButton::Left;
							break;
						case WM_RBUTTONDOWN:
							SetCapture(message->hwnd);
							mouse.State = MouseMessage::Down;
							mouse.Button = MouseButton::Right;
							break;
						case WM_RBUTTONUP:
							ReleaseCapture();
							mouse.State = MouseMessage::Up;
							mouse.Button = MouseButton::Right;
							break;
						case WM_MBUTTONDOWN:
							SetCapture(message->hwnd);
							mouse.State = MouseMessage::Down;
							mouse.Button = MouseButton::Middle;
							break;
						case WM_MBUTTONUP:
							ReleaseCapture();
							mouse.State = MouseMessage::Up;
							mouse.Button = MouseButton::Middle;
							break;
						case WM_MOUSEWHEEL:
							mouse.State = MouseMessage::Scroll;
							mouse.Location = lastMouseLocation; //not valid when scrolling
							mouse.Delta = -((short)HIWORD(message->wParam) / 120) * 4/*number of lines to scroll*/;
							break;
					}

					lastMouseLocation = mouse.Location;

					if (Application::Instance()->ProcessMouseMessage(mouse) == true)
					{
						return true;
					}
					break;
				}
				case WM_KEYDOWN:
				case WM_SYSKEYDOWN:
				case WM_KEYUP:
				case WM_SYSKEYUP:
				case WM_CHAR:
				case WM_SYSCHAR:
				case WM_IME_CHAR:
				{
					KeyboardMessage::KeyboardStates state = KeyboardMessage::Unknown;
					Misc::AnsiChar keyChar = '\0';
					Key::Keys keyData = Key::None;

					if (message->message == WM_CHAR || message->message == WM_SYSCHAR)
					{
						if (ImeWmCharsToIgnore > 0)
						{
							--ImeWmCharsToIgnore;
							return false;
						}
						else
						{
							state = KeyboardMessage::Character;
							keyChar = (Misc::AnsiChar)message->wParam;
						}
					}
					else if (message->message == WM_IME_CHAR)
					{
						int charSize = SystemDefaultCharSize;
					}
					else
					{
						Key::Keys modifier = Key::None;
						if (GetKeyState(Key::ControlKey) < 0)
							modifier |= Key::Control;
						if (GetKeyState(Key::ShiftKey) < 0)
							modifier |= Key::Shift;
						if (GetKeyState(Key::Menu) < 0)
							modifier |= Key::Alt;

						state = message->message == WM_KEYDOWN || message->message == WM_SYSKEYDOWN ? KeyboardMessage::KeyDown : KeyboardMessage::KeyUp;

						keyData = (Key::Keys)message->wParam | modifier;
					}

					if (state != KeyboardMessage::Unknown)
					{
						if (Application::Instance()->ProcessKeyboardMessage(KeyboardMessage(state, keyData, keyChar)) == true)
						{
							return true;
						}
					}

					break;
				}
			}

			return false;
		}
	}
}