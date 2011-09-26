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
			KeyDown,
			/**
			 * enthält das Zeichen der gedrückten Taste
			 */
			Character,
			/**
			 * Taste wurde losgelassen
			 */
			KeyUp
		};

		KeyboardStates State;
		Key::Keys KeyCode;
		Misc::UnicodeChar KeyChar;
		bool Menu,
			 Control,
			 Shift;
		
	public:
		KeyboardEvent() : Event(Event::Keyboard)
		{
			State = Unknown;
			Menu = false;
			Control = false;
			Shift = false;
			KeyCode = Key::None;
			KeyChar = L'\0';
		}
	
		KeyboardEvent(bool Menu, bool Control, bool Shift, Key::Keys KeyCode, Misc::UnicodeChar KeyChar) : Event(Event::Keyboard)
		{
			this->Menu = Menu;
			this->Control = Control;
			this->Shift = Shift;
			this->KeyCode = KeyCode;
			this->KeyChar = KeyChar;
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