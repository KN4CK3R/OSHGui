#ifndef OSHGUI_EVENT_KEYBOARDMESSAGE_HPP_
#define OSHGUI_EVENT_KEYBOARDMESSAGE_HPP_

#include "Key.hpp"
#include "Misc\Strings.hpp"

namespace OSHGui
{
	/**
	 * Tastaturevent
	 */
	class OSHGUI_EXPORT KeyboardMessage
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
		Misc::AnsiChar KeyChar;
		bool Menu,
			 Control,
			 Shift;
		
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		KeyboardMessage()
		{
			State = Unknown;
			Menu = false;
			Control = false;
			Shift = false;
			KeyCode = Key::None;
			KeyChar = '\0';
		}

		/**
		 * Prüft, ob das Zeichen alphanumerisch ist.
		 *
		 * @return ja / nein
		 */
		bool IsAlphaNumeric() const
		{
			return Misc::String::IsLetterOrDigit(KeyChar)
				|| Misc::String::IsPunctuation(KeyChar)
				|| Misc::String::IsSeperator(KeyChar)
				|| Misc::String::IsSymbol(KeyChar);
		}
	};
}

#endif