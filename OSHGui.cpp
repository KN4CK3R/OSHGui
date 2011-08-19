#include "OSHGui.h"

namespace OSHGui
{
	bool Gui::active = false;
	HHOOK Gui::messageHook = NULL;
	Gui* Gui::instance = NULL;
	Drawing::IFont* Gui::font = NULL;
	Drawing::IFont* Drawing::IRenderer::defaultFont = NULL;

	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Gui::Gui(Drawing::IRenderer *renderer)
	{
		if (renderer == NULL)
		{
			throw "Renderer can't be NULL";
		}

		instance = this;

		this->renderer = renderer;
		
		font = this->renderer->CreateNewFont();
		if (!font->Create(Misc::UnicodeString(L"Arial"), 13, false, false))
		{
			throw "Can't create default font.";
		}

		focusForm = NULL;
		focusForm = NULL;
	}
	//---------------------------------------------------------------------------
	Gui* Gui::GetInstance()
	{
		return instance;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void Gui::SetActive(bool active)
	{
		this->active = active;
	}
	//---------------------------------------------------------------------------
	bool Gui::GetActive()
	{
		return active;
	}
	//---------------------------------------------------------------------------
	Drawing::IFont* Gui::GetDefaultFont()
	{
		return font;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool Gui::InitMessageHook(HWND window)
	{
		if (window == NULL)
		{
			return false;
		}
	
		DWORD threadId = GetWindowThreadProcessId(window, NULL);
		if (threadId == 0x00)
		{
			return false;
		}
		
		messageHook = SetWindowsHookEx(WH_GETMESSAGE, MsgProc, NULL, threadId);
		if (messageHook == NULL)
		{
			return false;
		}
		
		return true;
	}
	//---------------------------------------------------------------------------
	bool Gui::RegisterForm(Form *form)
	{
		if (form == NULL)
		{
			return false;
		}
		
		for (unsigned int i = 0, len = forms.size(); i < len; i++)
		{
			if (forms.at(i) == form)
			{
				return true;
			}
		}
		
		forms.push_back(form);
		
		return true;
	}
	//---------------------------------------------------------------------------
	void Gui::UnregisterForm(Form *form)
	{
		if (form == NULL)
		{
			return;
		}
		
		for (unsigned int i = 0, len = forms.size(); i < len; i++)
		{
			if (forms.at(i) == form)
			{
				forms.erase(forms.begin() + i);
				
				if (focusForm == form)
				{
					if (forms.size() > 0)
					{
						focusForm = forms.at(0);
					}
					else
					{
						focusForm = NULL;
					}
				}
				
				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	void Gui::ShowMainForm(Form *form)
	{
		if (form == NULL)
		{
			return;
		}

		bool found = false;
		for (unsigned int i = 0; i < forms.size(); ++i)
		{
			if (forms.at(i) == form)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			return;
		}
		
		focusForm = form;
		
		focusForm->Show();
	}
	//---------------------------------------------------------------------------
	void Gui::Render()
	{
		if (!active)
		{
			return;
		}

		for (unsigned int i = 0, len = forms.size(); i < len; i++)
		{
			Form *form = forms.at(i);
			if (form != focusForm)
			{
				form->Render(renderer);
			}
		}
		focusForm->Render(renderer);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	LRESULT CALLBACK Gui::MsgProc(int code, WPARAM wParam, LPARAM lParam)
	{
		if (!active)
		{
			return CallNextHookEx(messageHook, code, wParam, lParam);
		}

		if (code == HC_ACTION)
		{
			return instance->ProcessMessage(code, wParam, lParam);
		}
		
		return CallNextHookEx(messageHook, code, wParam, lParam);
	}
	//---------------------------------------------------------------------------
	LRESULT Gui::ProcessMessage(int code, WPARAM wParam, LPARAM lParam)
	{
		if (focusForm == NULL)
		{
			return CallNextHookEx(messageHook, code, wParam, lParam);
		}

		LPMSG msg = (LPMSG)lParam;
		
		switch (msg->message)
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

				MouseEvent mouse(MouseEvent::None, Drawing::Point(GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam)), 0);

				switch (msg->message)
				{
					case WM_MOUSEMOVE:
						mouse.State = MouseEvent::Move;
						break;
					case WM_LBUTTONDOWN:
						SetCapture(msg->hwnd);
						mouse.State = MouseEvent::LeftDown;
						break;
					case WM_LBUTTONUP:
						ReleaseCapture();
						mouse.State = MouseEvent::LeftUp;
						break;
					case WM_RBUTTONDOWN:
						SetCapture(msg->hwnd);
						mouse.State = MouseEvent::RightDown;
						break;
					case WM_RBUTTONUP:
						ReleaseCapture();
						mouse.State = MouseEvent::RightUp;
						break;
					case WM_MOUSEWHEEL:
						mouse.State = MouseEvent::Scroll;
						mouse.Delta = -((short)HIWORD(msg->wParam) / 120) * 4/*number of lines to scroll*/;
						break;
				}
				
				if (focusForm->ProcessEvent(&mouse) == Event::DontContinue)
				{
					return true;
				}
				else
				{
					for (unsigned int i = 0, len = forms.size(); i < len; i++)
					{
						Form *form = forms.at(i);
						if (form != focusForm)
						{
							if (form->ProcessEvent(&mouse) == Event::DontContinue)
							{
								focusForm = form;

								return true;
							}
						}
					}
				}
			
				break;
			}
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
			case WM_CHAR:
			{
				KeyboardEvent keyboard;
				keyboard.State = KeyboardEvent::None;
				keyboard.Control = GetKeyState(VK_CONTROL) & 0x8000;
				keyboard.Shift = GetKeyState(VK_SHIFT) & 0x8000;
				keyboard.Menu = GetKeyState(VK_MENU) & 0x8000;

				if (msg->message == WM_KEYDOWN || msg->message == WM_SYSKEYDOWN || msg->message == WM_KEYUP || msg->message == WM_SYSKEYUP)
				{
					keyboard.State = msg->message == WM_KEYDOWN || msg->message == WM_SYSKEYDOWN ? KeyboardEvent::Down : KeyboardEvent::Up;

					switch (msg->wParam)
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
				else if (msg->message == WM_CHAR)
				{
					keyboard.State = KeyboardEvent::Character;

					switch ((Misc::UnicodeChar)msg->wParam)
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
							keyboard.KeyChar = (Misc::UnicodeChar)msg->wParam;
							switch (keyboard.KeyChar >= L'A' && keyboard.KeyChar <= L'Z' ? keyboard.KeyChar + 0x20 : keyboard.KeyChar)
							{
								case L'a':
									keyboard.KeyCode = Key::A;
									break;
								case L'b':
									keyboard.KeyCode = Key::B;
									break;
								case L'c':
									keyboard.KeyCode = Key::C;
									break;
								case L'd':
									keyboard.KeyCode = Key::D;
									break;
								case L'e':
									keyboard.KeyCode = Key::E;
									break;
								case L'f':
									keyboard.KeyCode = Key::F;
									break;
								case L'g':
									keyboard.KeyCode = Key::G;
									break;
								case L'h':
									keyboard.KeyCode = Key::H;
									break;
								case L'i':
									keyboard.KeyCode = Key::I;
									break;
								case L'j':
									keyboard.KeyCode = Key::J;
									break;
								case L'k':
									keyboard.KeyCode = Key::K;
									break;
								case L'l':
									keyboard.KeyCode = Key::L;
									break;
								case L'm':
									keyboard.KeyCode = Key::M;
									break;
								case L'n':
									keyboard.KeyCode = Key::N;
									break;
								case L'o':
									keyboard.KeyCode = Key::O;
									break;
								case L'p':
									keyboard.KeyCode = Key::P;
									break;
								case L'q':
									keyboard.KeyCode = Key::Q;
									break;
								case L'r':
									keyboard.KeyCode = Key::R;
									break;
								case L's':
									keyboard.KeyCode = Key::S;
									break;
								case L't':
									keyboard.KeyCode = Key::T;
									break;
								case L'u':
									keyboard.KeyCode = Key::U;
									break;
								case L'v':
									keyboard.KeyCode = Key::V;
									break;
								case L'w':
									keyboard.KeyCode = Key::W;
									break;
								case L'x':
									keyboard.KeyCode = Key::X;
									break;
								case L'y':
									keyboard.KeyCode = Key::Y;
									break;
								case L'z':
									keyboard.KeyCode = Key::Z;
									break;
								default:
									keyboard.KeyCode = Key::None;
									break;
							}
							break;
					}
				}
				if (keyboard.State == KeyboardEvent::None)
				{
					break;
				}
			
				if (focusForm->ProcessEvent(&keyboard) == Event::DontContinue)
				{
					return true;
				}
			
				break;
			}
		}

		return CallNextHookEx(messageHook, code, wParam, lParam);
	}
	//---------------------------------------------------------------------------
}