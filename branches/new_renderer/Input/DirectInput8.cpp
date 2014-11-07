/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "DirectInput8.hpp"
#include "../Event/MouseMessage.hpp"
#include "../Event/KeyboardMessage.hpp"
#include "../Application.hpp"

namespace OSHGui
{
	namespace Input
	{
		DirectInput8::DirectInput8()
			: directInputInterface(nullptr),
			  mouseDevice(nullptr),
			  keyboardDevice(nullptr),
			  lastMouseButtonStates{}
		{

		}
		//---------------------------------------------------------------------------
		DirectInput8::~DirectInput8()
		{
			Release();
		}
		//---------------------------------------------------------------------------
		void DirectInput8::SetMouseClipArea(const Drawing::RectangleI &area)
		{
			mouseClipArea = area;
		}
		//---------------------------------------------------------------------------
		bool DirectInput8::Initialize(HINSTANCE instance, HWND target)
		{
			if (DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8A, (void**)&directInputInterface, nullptr) != DI_OK)
			{
				return false;
			}

			//mouse
			if (directInputInterface->CreateDevice(GUID_SysMouse, &mouseDevice, nullptr) != DI_OK)
			{
				Release();
				return false;
			}
			if (mouseDevice->SetDataFormat(&c_dfDIMouse2) != DI_OK)
			{
				Release();
				return false;
			}
			if (mouseDevice->SetCooperativeLevel(target, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) != DI_OK)
			{
				Release();
				return false;
			}

			return true;
		}
		//---------------------------------------------------------------------------
		void DirectInput8::Release()
		{
			if (mouseDevice)
			{
				mouseDevice->Unacquire();
				mouseDevice->Release();
				mouseDevice = nullptr;
			}
			if (keyboardDevice)
			{
				keyboardDevice->Unacquire();
				keyboardDevice->Release();
				keyboardDevice = nullptr;
			}
			if (directInputInterface)
			{
				directInputInterface->Release();
				directInputInterface = nullptr;
			}
		}
		//---------------------------------------------------------------------------
		bool DirectInput8::ProcessDevices()
		{
			ProcessMouseDevice();

			return true;
		}
		//---------------------------------------------------------------------------
		void DirectInput8::ProcessMouseDevice()
		{
			DIMOUSESTATE2 mouseState;
			auto result = mouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
			if (result != DI_OK)
			{
				if (result == DIERR_NOTACQUIRED || result == DIERR_INPUTLOST)
				{
					mouseDevice->Acquire();
				}
				return;
			}

			if (mouseState.lX != 0 || mouseState.lY != 0)
			{
				mouseLocation.X += mouseState.lX;
				mouseLocation.Y += mouseState.lY;

				if (mouseLocation.X < mouseClipArea.GetLeft())
				{
					mouseLocation.X = mouseClipArea.GetLeft();
				}
				if (mouseLocation.X > mouseClipArea.GetRight())
				{
					mouseLocation.X = mouseClipArea.GetRight();
				}
				if (mouseLocation.Y < mouseClipArea.GetTop())
				{
					mouseLocation.Y = mouseClipArea.GetTop();
				}
				if (mouseLocation.Y > mouseClipArea.GetBottom())
				{
					mouseLocation.Y = mouseClipArea.GetBottom();
				}

				InjectMouseMessage(MouseMessage(MouseState::Move, MouseButton::None, mouseLocation, 0));
			}
			if (mouseState.lZ != 0)
			{
				InjectMouseMessage(MouseMessage(MouseState::Scroll, MouseButton::None, mouseLocation, mouseState.lZ));
			}
			for (auto i = 0; i < 5; ++i)
			{
				if (mouseState.rgbButtons[i] != lastMouseButtonStates[i])
				{
					InjectMouseMessage(MouseMessage((mouseState.rgbButtons[i] & 0x80) ? MouseState::Down : MouseState::Up, (MouseButton)((int)MouseButton::Left + i), mouseLocation, 0));
				}
			}
			std::memcpy(lastMouseButtonStates, mouseState.rgbButtons, 5);
		}
		//---------------------------------------------------------------------------
		bool DirectInput8::InjectMouseMessage(MouseMessage &&mouse)
		{
			return Application::Instance().ProcessMouseMessage(mouse);
		}
		//---------------------------------------------------------------------------
		bool DirectInput8::InjectKeyboardMessage(KeyboardMessage &&keyboard)
		{
			return Application::Instance().ProcessKeyboardMessage(keyboard);
		}
		//---------------------------------------------------------------------------
	}
}