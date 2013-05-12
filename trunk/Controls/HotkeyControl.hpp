/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_HOTKEYCONTROL_HPP
#define OSHGUI_HOTKEYCONTROL_HPP

#include "TextBox.hpp"
#include <map>

//TODO: inheritance of TextBox is wrong => should be a composition with a TextBox member => need to reimplement all Control methods

namespace OSHGui
{
	/**
	 * Tritt ein, wenn sich der Wert der Hotkey/Modifier-Eigenschaft �ndert.
	 */
	typedef Event<void(Control*)> HotkeyChangedEvent;
	typedef EventHandler<void(Control*)> HotkeyChangedEventHandler;

	/**
	 * Stellt ein Textfeld-Steuerelement dar.
	 */
	class OSHGUI_EXPORT HotkeyControl : public TextBox
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		HotkeyControl();
		virtual ~HotkeyControl();
		
		virtual void SetText(const Misc::AnsiString &text) override;

		/**
		 * Legt den Hotkey fest.
		 *
		 * @param hotkey
		 */
		virtual void SetHotkey(Key::Keys hotkey);
		/**
		 * Ruft den Hotkey ab.
		 *
		 * @return hotkey
		 */
		virtual Key::Keys GetHotkey() const;
		/**
		 * Legt den HotkeyModifier fest.
		 *
		 * @param hotkey
		 */
		virtual void SetHotkeyModifier(Key::Keys modifier);
		/**
		 * Ruft den HotkeyModifier ab.
		 *
		 * @return hotkeyModifier
		 */
		virtual Key::Keys GetHotkeyModifier() const;

		/**
		 * Ruft das HotkeyChangedEvent f�r das Steuerelement ab.
		 *
		 * @return hotkeyChangedEvent
		 */
		HotkeyChangedEvent& GetHotkeyChangedEvent();
		
		/**
		 * Setzt den Hotkey und den HotkeyModifier auf None.
		 */
		void ClearHotkey();

		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation() override;

		virtual void ShowCaret(bool showCaret);

		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer) override;
	
	protected:
		virtual void OnMouseClick(const MouseMessage &mouse) override;
		virtual bool OnKeyDown(const KeyboardMessage &keyboard) override;
		virtual bool OnKeyPress(const KeyboardMessage &keyboard) override;
		virtual bool OnKeyUp(const KeyboardMessage &keyboard) override;

	private:
		void HotkeyToText();

		Drawing::Point clearButtonAbsoluteLocation;

		Key::Keys hotkey;
		Key::Keys modifier;

		HotkeyChangedEvent hotkeyChangedEvent;
		
		static std::map<Key::Keys, Misc::AnsiString> hotkeyNames;
	};
}

#endif