#include <locale>
#include "Windows.hpp"
#include "..\Event\MouseMessage.hpp"
#include "..\Event\KeyboardMessage.hpp"
#include "..\Application.hpp"

namespace OSHGui
{
	namespace Input
	{
		bool Windows::ProcessMessage(LPMSG message)
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

					MouseMessage mouse(MouseMessage::Unknown, Drawing::Point(GET_X_LPARAM(message->lParam), GET_Y_LPARAM(message->lParam)), 0);

					switch (message->message)
					{
						case WM_MOUSEMOVE:
							mouse.State = MouseMessage::Move;
							break;
						case WM_LBUTTONDOWN:
							SetCapture(message->hwnd);
							mouse.State = MouseMessage::LeftDown;
							break;
						case WM_LBUTTONUP:
							ReleaseCapture();
							mouse.State = MouseMessage::LeftUp;
							break;
						case WM_RBUTTONDOWN:
							SetCapture(message->hwnd);
							mouse.State = MouseMessage::RightDown;
							break;
						case WM_RBUTTONUP:
							ReleaseCapture();
							mouse.State = MouseMessage::RightUp;
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
				{
					KeyboardMessage keyboard;
					keyboard.State = KeyboardMessage::Unknown;
					keyboard.Control = (GetKeyState(VK_CONTROL) & 0x8000) > 0;
					keyboard.Shift = (GetKeyState(VK_SHIFT) & 0x8000) > 0;
					keyboard.Menu = (GetKeyState(VK_MENU) & 0x8000) > 0;

					if (message->message == WM_KEYDOWN || message->message == WM_SYSKEYDOWN || message->message == WM_KEYUP || message->message == WM_SYSKEYUP)
					{
						keyboard.State = message->message == WM_KEYDOWN || message->message == WM_SYSKEYDOWN ? KeyboardMessage::KeyDown : KeyboardMessage::KeyUp;

						switch (message->wParam)
						{
							case VK_CONTROL:
								keyboard.KeyCode = Key::Control;
								break;
							case VK_SHIFT:
								keyboard.KeyCode = Key::Shift;
								break;
							case VK_MENU:
								keyboard.KeyCode = Key::Alt;
								break;
							case VK_BACK:
								keyboard.KeyCode = Key::Back;
								break;
							case VK_SPACE:
								keyboard.KeyCode = Key::Space;
								keyboard.KeyChar = ' ';
								break;
							case VK_TAB:
								keyboard.KeyCode = Key::Tab;
								break;
							case VK_CLEAR:
							case VK_DELETE:
								keyboard.KeyCode = Key::Delete;
								break;
							case VK_INSERT:
								keyboard.KeyCode = Key::Insert;
								break;
							case VK_RETURN:
								keyboard.KeyCode = Key::Return;
								break;
							case VK_PRIOR:
								keyboard.KeyCode = Key::PageDown;
								break;
							case VK_NEXT:
								keyboard.KeyCode = Key::PageUp;
								break;
							case VK_END:
								keyboard.KeyCode = Key::End;
								break;
							case VK_HOME:
								keyboard.KeyCode = Key::Home;
								break;
							case VK_LEFT:
								keyboard.KeyCode = Key::Left;
								break;
							case VK_UP:
								keyboard.KeyCode = Key::Up;
								break;
							case VK_RIGHT:
								keyboard.KeyCode = Key::Right;
								break;
							case VK_DOWN:
								keyboard.KeyCode = Key::Down;
								break;
							case 26:
							case 1:
							case 2:
							case 14:
							case 19:
							case 4:
							case 6:
							case 7:
							case 10:
							case 11:
							case 23:
							case 5:
							case 20:
							case 25:
							case 21:
							case 22:
							case 24:
							case 15:
							case 27:
							case 29:
							case 28:
								keyboard.KeyCode = Key::None;
								break;
						}
					}
					else if (message->message == WM_CHAR)
					{
						keyboard.State = KeyboardMessage::Character;

						switch ((Misc::AnsiChar)message->wParam)
						{
							case VK_BACK:
								keyboard.KeyCode = Key::Back;
								break;
							case 24:        // Ctrl-X Cut
							case VK_CANCEL: // Ctrl-C Copy
								break;
							case 22: // Ctrl-V Paste
								break;
							case 1: // Ctrl-A Select All
								break;
							case 26:  // Ctrl Z
							case 2:   // Ctrl B
							case 14:  // Ctrl N
							case 19:  // Ctrl S
							case 4:   // Ctrl D
							case 6:   // Ctrl F
							case 7:   // Ctrl G
							case 10:  // Ctrl J
							case 11:  // Ctrl K
							case 12:  // Ctrl L
							case 17:  // Ctrl Q
							case 23:  // Ctrl W
							case 5:   // Ctrl E
							case 18:  // Ctrl R
							case 20:  // Ctrl T
							case 25:  // Ctrl Y
							case 21:  // Ctrl U
							case 9:   // Ctrl I
							case 15:  // Ctrl O
							case 16:  // Ctrl P
							case 27:  // Ctrl [
							case 29:  // Ctrl ]
							case 28:  // Ctrl \ 
								break;
							default:
								keyboard.KeyChar = (Misc::AnsiChar)message->wParam;
								std::locale loc;
								Misc::AnsiChar keyChar = std::tolower(keyboard.KeyChar, loc);
								
								if (keyChar >= 'a' && keyChar <= 'z')
								{
									keyboard.KeyCode = (Key::Keys)((int)Key::A + (keyChar - 'a'));
								}
								else
								{
									keyboard.KeyCode = Key::None;
								}
								break;
						}
					}
					if (keyboard.State == KeyboardMessage::Unknown)
					{
						break;
					}
			
					if (Application::Instance()->ProcessKeyboardMessage(keyboard) == true)
					{
						return true;
					}
					break;
				}
			}

			return false;
		}
	}
}