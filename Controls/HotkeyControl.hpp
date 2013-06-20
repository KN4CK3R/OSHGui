/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_HOTKEYCONTROL_HPP
#define OSHGUI_HOTKEYCONTROL_HPP

#include "Control.hpp"
#include <map>

namespace OSHGui
{
	/**
	 * Tritt ein, wenn sich der Wert der Hotkey/Modifier-Eigenschaft ändert.
	 */
	typedef Event<void(Control*)> HotkeyChangedEvent;
	typedef EventHandler<void(Control*)> HotkeyChangedEventHandler;

	class TextBox;
	
	/**
	 * Stellt ein Textfeld-Steuerelement dar.
	 */
	class OSHGUI_EXPORT HotkeyControl : public Control
	{
	public:
		using Control::SetLocation;
		using Control::SetSize;
	
		/**
		 * Konstruktor der Klasse.
		 */
		HotkeyControl();
		virtual ~HotkeyControl();

		/**
		 * Legt die Koordinaten der linken oberen Ecke des Steuerelements relativ zur
		 * linken oberen Ecke des Containers fest.
		 *
		 * @param location
		 */
		virtual void SetLocation(const Drawing::Point &location);
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size);
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * @param font
		 */
		virtual void SetFont(const std::shared_ptr<Drawing::IFont> &font) override;
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetForeColor(Drawing::Color color);
		/**
		 * Legt die Hintergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetBackColor(Drawing::Color color);
		
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
		 * Ruft das HotkeyChangedEvent für das Steuerelement ab.
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

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
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

		static const Drawing::Size DefaultSize;
		static std::map<Key::Keys, Misc::AnsiString> hotkeyNames;
		
		TextBox *textBox;
		
		Drawing::Point clearButtonAbsoluteLocation;

		Key::Keys hotkey;
		Key::Keys modifier;

		HotkeyChangedEvent hotkeyChangedEvent;
	};
}

#endif