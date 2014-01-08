/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EVENT_KEYBOARDMESSAGE_HPP
#define OSHGUI_EVENT_KEYBOARDMESSAGE_HPP

#include "Key.hpp"
#include "../Misc/Strings.hpp"

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
		
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		KeyboardMessage()
			: state(Unknown),
			  keyData(Key::None),
			  keyChar('\0')
		{

		}

		KeyboardMessage(KeyboardStates state, Key::Keys keyData, Misc::AnsiChar keyChar)
			: state(state),
			  keyData(keyData),
			  keyChar(keyChar)
		{

		}

		Key::Keys GetKeyData() const
		{
			return keyData;
		}

		Key::Keys GetKeyCode() const
		{
			return keyData & Key::KeyCode;
		}

		Key::Keys GetModifier() const
		{
			return keyData & Key::Modifiers;
		}

		KeyboardStates GetState() const
		{
			return state;
		}

		Misc::AnsiChar GetKeyChar() const
		{
			return keyChar;
		}

		/**
		 * Prüft, ob das Zeichen alphanumerisch ist.
		 *
		 * \return ja / nein
		 */
		bool IsAlphaNumeric() const
		{
			return Misc::String::IsLetterOrDigit(keyChar)
				|| Misc::String::IsPunctuation(keyChar)
				|| Misc::String::IsSeperator(keyChar)
				|| Misc::String::IsSymbol(keyChar);
		}

	private:
		KeyboardStates state;
		Misc::AnsiChar keyChar;
		Key::Keys keyData;

	};
}

#endif