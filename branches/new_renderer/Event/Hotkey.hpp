/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EVENT_HOTKEY_HPP
#define OSHGUI_EVENT_HOTKEY_HPP

#include "../Exports.hpp"
#include "../Misc/Exceptions.hpp"
#include "Key.hpp"

namespace OSHGui
{
	/**
	 * Stellt die Methode dar, die ein Ereignis behandelt.
	 */
	class OSHGUI_EXPORT Hotkey
	{
	private:
		typedef std::function<void()> Handler;
		Key::Keys key;
		Key::Keys modifier;
		Handler handler;

	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * \param key
		 * \param handler die aufzurufende Funktion
		 */
		Hotkey(Key::Keys key, const Handler &handler)
			: key(key),
			  modifier(Key::None),
			  handler(handler)
		{

		}

		/**
		 * Konstruktor der Klasse.
		 *
		 * \param key
		 * \param modifier Strg / Alt / Shift
		 * \param handler die aufzurufende Funktion
		 */
		Hotkey(Key::Keys key, Key::Keys modifier, const Handler &handler)
			: key(key),
			  modifier(modifier),
			  handler(handler)
		{
			if (modifier == key || !(modifier == Key::None || modifier == Key::Control || modifier == Key::Alt || modifier == Key::Shift))
			{
				#ifndef OSHGUI_DONTUSEEXCEPTIONS
				throw Misc::ArgumentException("modifier");
				#endif
				throw 1;
			}
		}
		
		/**
		 * CopyKonstruktor der Klasse.
		 *
		 * \param hotkey
		 */
		Hotkey(const Hotkey &hotkey)
			: key(hotkey.key),
			  modifier(hotkey.modifier),
			  handler(hotkey.handler)
		{
			
		}

		/**
		 * Ruft die Taste des Hotkeys ab.
		 *
		 * \return die Taste
		 */
		Key::Keys GetKey() const
		{
			return key;
		}
		/**
		 * Ruft den Modifier des Hotkeys ab.
		 *
		 * \return der Modifier
		 */
		Key::Keys GetModifier() const
		{
			return modifier;
		}

		bool operator==(const Hotkey &hotkey)
		{
			return key == hotkey.key && modifier == hotkey.modifier;
		}

		void operator()()
		{
			if (handler)
			{
				handler();
			}
		}
		
		Hotkey& operator=(const Hotkey &hotkey)
		{
			if (this != &hotkey)
			{
				key = hotkey.key;
				modifier = hotkey.modifier;
				handler = hotkey.handler;
			}
			return *this;
		}
	};
}

#endif