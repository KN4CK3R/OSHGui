#include "OSHGui.h"

namespace OSHGui
{
	//LRESULT CALLBACK MsgProc(int code, WPARAM wParam, LPARAM lParam);
	bool Gui::active = false;
	HHOOK Gui::messageHook = NULL;
	Gui *Gui::self = NULL;
	Misc::TimeHelper Gui::GlobalTime;

	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Gui::Gui()
	{
		self = this;
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
		
		messageHook = SetWindowsHookEx(WH_CALLWNDPROC, MsgProc, NULL, threadId);
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
		
		for (int i = 0, len = forms.GetSize(); i < len; i++)
		{
			if (forms.Get(i) == form)
			{
				return true;
			}
		}
		
		forms.Add(form);
		
		return true;
	}
	//---------------------------------------------------------------------------
	void Gui::UnregisterForm(Form *form)
	{
		if (form == NULL)
		{
			return;
		}
		
		for (int i = 0, len = forms.GetSize(); i < len; i++)
		{
			if (forms.Get(i) == form)
			{			
				forms.Remove(i);
				
				if (mainForm == form)
				{
					if (forms.GetSize() > 0)
					{
						mainForm = forms.Get(0);
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
		if (form == NULL || !forms.Contains(form))
		{
			return;
		}
		
		mainForm = form;
		
		form->SetVisible(true);
		form->SetEnabled(true);
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
			return self->ProcessMessage(code, wParam, lParam);
		}
		
		return CallNextHookEx(messageHook, code, wParam, lParam);
	}
	//---------------------------------------------------------------------------
	LRESULT Gui::ProcessMessage(int code, WPARAM wParam, LPARAM lParam)
	{
		LPCWPRETSTRUCT wpret = (LPCWPRETSTRUCT)lParam;
		
		switch (wpret->message)
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

				MouseEvent mouse(MouseEvent::None, Drawing::Point((int)(short)LOWORD(wpret->lParam), GET_Y_LPARAM(wpret->lParam)), 0);
				
				switch (wpret->message)
				{
					case WM_MOUSEMOVE:
						mouse.State = MouseEvent::Move;
						break;
					case WM_LBUTTONDOWN:
						mouse.State = MouseEvent::LeftDown;
						break;
					case WM_LBUTTONUP:
						mouse.State = MouseEvent::LeftUp;
						break;
					case WM_RBUTTONDOWN:
						mouse.State = MouseEvent::RightDown;
						break;
					case WM_RBUTTONUP:
						mouse.State = MouseEvent::RightUp;
						break;
					case WM_MOUSEWHEEL:
						mouse.State = MouseEvent::Wheel;
						UINT lines;
						SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &lines, 0);
						mouse.Delta = lines * GET_WHEEL_DELTA_WPARAM(wpret->wParam);
						break;
				}
				
				if (focusForm->ProcessEvent(&mouse) == Event::None)
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
				keyboard.Control = GetKeyState(VK_CONTROL) >= 0;
				keyboard.Shift = GetKeyState(VK_SHIFT) >= 0;
				keyboard.Menu = GetKeyState(VK_MENU) >= 0;
			
				switch (wpret->message)
				{
					case WM_KEYDOWN:
					case WM_SYSKEYDOWN:
					
					case WM_KEYUP:
					case WM_SYSKEYUP:
					
					case WM_CHAR:
					{
						switch ((char)wpret->wParam)
						{
							case VK_BACK:
								keyboard.KeyCode = Key::Back;
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
								keyboard.KeyChar = (char)wpret->wParam;
						}
					}
				}
			
				if (focusForm->ProcessEvent(&keyboard) == Event::None)
				{
					return true;
				}
			
				break;
			}
			case WM_ACTIVATEAPP:
			case WM_CAPTURECHANGED:
			{
				SystemEvent system;
				
				switch (wpret->message)
				{
					case WM_ACTIVATEAPP:
						system.State = SystemEvent::ActivateApp;
						system.Value = wpret->lParam == 1;
						break;
					case WM_CAPTURECHANGED:
						system.State = SystemEvent::CaptureChanged;
						system.Value = true;
						break;
				}
				
				if (focusForm->ProcessEvent(&system) == Event::None)
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