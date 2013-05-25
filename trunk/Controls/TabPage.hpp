/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_TABPAGE_HPP
#define OSHGUI_TABPAGE_HPP

#include "Panel.hpp"
#include "TabControl.hpp"

namespace OSHGui
{
	/**
	 * Wird zum Gruppieren von Auflistungen von Steuerelementen verwendet.
	 */
	class OSHGUI_EXPORT TabPage : public Panel
	{
		friend TabControl;

	public:
		using Panel::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		TabPage();
		
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size) override;
		/**
		 * Legt das übergeordnete Steuerelement fest.
		 * Kann nur ein TabControl sein!
		 *
		 * @param parent
		 */
		virtual void SetParent(Control *parent) override;
		/**
		 * Legt den Text fest.
		 *
		 * @param text
		 */
		void SetText(const Misc::AnsiString &text);
		/**
		 * Ruft den Text ab.
		 *
		 * @return der Text
		 */
		const Misc::AnsiString& GetText() const;
		/**
		 * Gibt eine Liste der untergeordneten Steuerelemente zurück.
		 *
		 * @return parent
		 */
		virtual const std::deque<Control*>& GetControls() const override;

		/**
		 * Fügt ein untergeordnetes Steuerelement hinzu.
		 *
		 * @param control
		 */
		virtual void AddControl(Control *control) override;
		/**
		 * Entfernt ein untergeordnetes Steuerelement.
		 *
		 * @param control
		 */
		virtual void RemoveControl(Control *control) override;

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer) override;

	private:
		Misc::AnsiString text;

		Panel *containerPanel;
		TabControl::TabControlButton *button;
	};
}

#endif