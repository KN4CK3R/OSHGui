#ifndef __OSHGUI_KEYBOARDEVENT_H__
#define __OSHGUI_KEYBOARDEVENT_H__

#include "Key.h"

namespace OSHGui
{
	class KeyboardEvent : public Event
	{
	public:
		enum KeyboardStates { None, Down, Character, Up };

		KeyboardStates State;
		Key::Keys KeyCode;
		char KeyChar;
		bool Menu,
			 Control,
			 Shift;
		bool Handled;
		
	public:
		KeyboardEvent() : Event(Event::Keyboard)
		{
			State = None;
			Menu = false;
			Control = false;
			Shift = false;
			KeyCode = Key::None;
			KeyChar = 0;
			Handled = false;
		}
	
		KeyboardEvent(bool Menu, bool Control, bool Shift, Key::Keys KeyCode, char KeyChar) : Event(Event::Keyboard)
		{
			this->Menu = Menu;
			this->Control = Control;
			this->Shift = Shift;
			this->KeyCode = KeyCode;
			this->KeyChar = KeyChar;
			Handled = false;
		}
		
		char GetCharacter()
		{
			if (Key::D0 <= KeyCode && KeyCode <= Key::D9)
			{
				return '0' + KeyCode;
			}
			if (Key::A <= KeyCode && KeyCode <= Key::Z)
			{
				if (Shift)
				{
					return 'A' + KeyCode;
				}
				else
				{
					return 'a' + KeyCode;
				}
			}
			if (Key::Space == KeyCode)
			{
				return ' ';
			}
			
			switch (KeyCode)
			{
				case Key::Divide:
					return '/';
				case Key::Multiply:
					return '*';
				case Key::Substract:
					return '-';
				case Key::Add:
					return '+';
			}
			
			return '\0';
		}

		bool IsAlphaNumeric()
		{
			return KeyChar != 0;
		}
	};
}

#endif