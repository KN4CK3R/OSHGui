#include "OSHGui.h"

namespace OSHGui
{
	bool Gui::active = false;
	HHOOK Gui::messageHook = NULL;
	Gui* Gui::instance = NULL;
	Drawing::IFont* Gui::font = NULL;
	Misc::TimeHelper Gui::GlobalTime;
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

		mainForm = NULL;
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
				//forms.Remove(i);
				
				if (mainForm == form)
				{
					if (forms.size() > 0)
					{
						mainForm = forms.at(0);
					}
					else
					{
						mainForm = NULL;
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
		
		mainForm = form;
		focusForm = mainForm;
		
		mainForm->Show();
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
			if (form != mainForm)
			{
				form->Render(renderer);
			}
		}
		mainForm->Render(renderer);
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
						mouse.State = MouseEvent::Wheel;
						/*UINT lines;
						SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &lines, 0);
						mouse.Delta = lines * GET_WHEEL_DELTA_WPARAM(msg->wParam);*/
						mouse.Delta = (short)HIWORD(msg->wParam);
						break;
				}
				
				if (focusForm->ProcessEvent(&mouse) == Event::DontContinue)
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
				KeyboardEvent keyboard;
				keyboard.State = KeyboardEvent::None;
				keyboard.Control = GetKeyState(VK_CONTROL) & 0x8000;
				keyboard.Shift = GetKeyState(VK_SHIFT) & 0x8000;
				keyboard.Menu = GetKeyState(VK_MENU) & 0x8000;

				if (msg->message == WM_KEYDOWN || msg->message == WM_SYSKEYDOWN)
				{
					keyboard.State = KeyboardEvent::Down;
				}
				else if (msg->message == WM_KEYUP || msg->message == WM_SYSKEYUP)
				{
					keyboard.State = KeyboardEvent::Up;
				}
				else if (msg->message == WM_CHAR)
				{
					keyboard.State = KeyboardEvent::Character;
				}
				if (keyboard.State == KeyboardEvent::DontContinue)
				{
					break;
				}
			
				switch ((char)msg->wParam)
				{
					case VK_BACK:
						keyboard.KeyCode = Key::Back;
						keyboard.KeyChar = '\b';
						break;
					case VK_SPACE:
						keyboard.KeyCode = Key::Space;
						keyboard.KeyChar = ' ';
						break;
					case 26:
					case 2:
					case 14:
					case 19:
					case 4:
					case 6:
					case 7:
					case 10:
					case 11:
					case 12:
					case 17:
					case 23:
					case 5:
					case 18:
					case 20:
					case 25:
					case 21:
					case 9:
					case 15:
					case 16:
					case 27:
					case 29:
					case 28:
						keyboard.KeyCode = Key::None;
						break;
					default:
						keyboard.KeyChar = (char)msg->wParam;
				}
			
				if (focusForm->ProcessEvent(&keyboard) == Event::DontContinue)
				{
					return true;
				}
			
				break;
			}
			case WM_ACTIVATEAPP:
			case WM_CAPTURECHANGED:
			{
				SystemEvent system;
				
				switch (msg->lParam)
				{
					case WM_ACTIVATEAPP:
						system.State = SystemEvent::ActivateApp;
						system.Value = msg->message == 1;
						break;
					case WM_CAPTURECHANGED:
						system.State = SystemEvent::CaptureChanged;
						system.Value = true;
						break;
				}
				
				if (focusForm->ProcessEvent(&system) == Event::DontContinue)
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