#ifndef OSHGUI_EVENT_KEYBOARDMESSAGE_HPP_
#define OSHGUI_EVENT_KEYBOARDMESSAGE_HPP_

#include "Key.hpp"
#include "IEvent.hpp"
#include "Misc\Strings.hpp"

namespace OSHGui
{
	/**
	 * Tastaturevent
	 */
	class OSHGUI_EXPORT KeyboardMessage : public IEvent
	{
	public:
		enum KeyboardStates
		{
			/**
			 * Unbekannt
			 */
			Unknown,
			/**
			 * Taste wurde gedr�ckt
			 */
			KeyDown,
			/**
			 * enth�lt das Zeichen der gedr�ckten Taste
			 */
			Character,
			/**
			 * Taste wurde losgelassen
			 */
			KeyUp
		};

		KeyboardStates State;
		Key::Keys KeyCode;
		Misc::AnsiChar KeyChar;
		bool Menu,
			 Control,
			 Shift;
		
	public:
		KeyboardMessage() : IEvent(IEvent::Keyboard)
		{
			State = Unknown;
			Menu = false;
			Control = false;
			Shift = false;
			KeyCode = Key::None;
			KeyChar = '\0';
		}
	
		KeyboardMessage(bool Menu, bool Control, bool Shift, Key::Keys KeyCode, Misc::AnsiChar KeyChar) : IEvent(IEvent::Keyboard)
		{
			this->Menu = Menu;
			this->Control = Control;
			this->Shift = Shift;
			this->KeyCode = KeyCode;
			this->KeyChar = KeyChar;
		}

		/**
		 * Pr�ft, ob das Zeichen alphanumerisch ist.
		 *
		 * @return ja / nein
		 */
		bool IsAlphaNumeric() const
		{
			return Misc::String::IsLetterOrDigit(KeyChar) || Misc::String::IsPunctuation(KeyChar) || Misc::String::IsSeperator(KeyChar) || Misc::String::IsSymbol(KeyChar);
		}
	};
}

#endif