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

namespace OSHGui
{
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

		virtual void ShowCaret(bool showCaret);

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		//virtual void Render(Drawing::IRenderer *renderer) override;
	
	protected:
		virtual bool OnKeyDown(const KeyboardMessage &keyboard) override;
		virtual bool OnKeyPress(const KeyboardMessage &keyboard) override;
		virtual bool OnKeyUp(const KeyboardMessage &keyboard) override;

	private:
		void ClearHotkey();
		void HotkeyToText();

		Key::Keys hotkey;
		Key::Keys modifier;
	};
}

#endif