#ifndef __OSHGUI_KEYBOARDEVENT_H__
#define __OSHGUI_KEYBOARDEVENT_H__

#include "Key.h"
#include "Event.h"
#include "Misc\Strings.h"

namespace OSHGui
{
	/**
	 * Tastaturevent
	 */
	class KeyboardEvent : public Event
	{
	public:
		enum KeyboardStates
		{
			/**
			 * Unbekannt
			 */
			Unknown,
			/**
			 * Taste wurde gedrückt
			 */
			Down,
			/**
			 * enthält das Zeichen der gedrückten Taste
			 */
			Character,
			/**
			 * Taste wurde losgelassen
			 */
			Up
		};

		KeyboardStates State;
		Key::Keys KeyCode;
		Misc::UnicodeChar KeyChar;
		bool Menu,
			 Control,
			 Shift;
		bool Handled;
		
	public:
		KeyboardEvent() : Event(Event::Keyboard)
		{
			State = Unknown;
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

		/**
		 * Prüft, ob das Zeichen alphanumerisch ist.
		 *
		 * @return ja / nein
		 */
		bool IsAlphaNumeric() const
		{
			return KeyChar != 0;
		}
	};
}

#endif