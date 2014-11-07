/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_INPUT_DIRECTINPUT8_HPP
#define OSHGUI_INPUT_DIRECTINPUT8_HPP

//#include <Windows.h>
#include <dinput.h>
#include "../Drawing/Rectangle.hpp"
#include "Input.hpp"

namespace OSHGui
{
	namespace Input
	{
		/**
		 * Verwaltet den Input von DirectInput8.
		 */
		class OSHGUI_EXPORT DirectInput8 : public Input
		{
		public:
			DirectInput8();
			~DirectInput8();

			bool Initialize(HINSTANCE instance, HWND target);
			void Release();

			void SetMouseClipArea(const Drawing::RectangleI &area);

			/**
			 * Ruft den Status der Geräte ab und erzeugt Eingabe Nachrichten.
			 *
			 * \param message
			 */
			bool ProcessDevices();

		private:
			void ProcessMouseDevice();
			void ProcessKeyboardDevice();

			IDirectInput8 *directInputInterface;
			IDirectInputDevice8A *keyboardDevice;
			IDirectInputDevice8A *mouseDevice;

			Drawing::RectangleI mouseClipArea;
			Drawing::PointI mouseLocation;
			BYTE lastMouseButtonStates[5];
		};
	}
}

#endif