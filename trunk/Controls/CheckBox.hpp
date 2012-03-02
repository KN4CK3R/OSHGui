/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_CHECKBOX_HPP
#define OSHGUI_CHECKBOX_HPP

#include "Control.hpp"
#include "Label.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein CheckBox-Steuerelement dar.
	 */
	class OSHGUI_EXPORT CheckBox : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		CheckBox();
		virtual ~CheckBox();
		
		/**
		 * Legt den checked-Status fest.
		 *
		 * @param checked
		 */
		virtual void SetChecked(bool checked);
		/**
		 * Gibt den checked-Status zurück.
		 *
		 * @return ja / nein
		 */
		virtual bool GetChecked() const;
		/**
		 * Legt den Text fest. Falls autoSize = true, wird die Größe automatisch angepasst.
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
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * @param font
		 */
		virtual void SetFont(const std::shared_ptr<Drawing::IFont> &font);
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetForeColor(const Drawing::Color &color);
		/**
		 * Ruft das CheckedChangeEvent für das Steuerelement ab.
		 *
		 * @return checkedChangeEvent
		 */
		CheckedChangedEvent& GetCheckedChangedEvent();
		
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation();
		
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
	
	protected:
		static const Drawing::Size DefaultLabelOffset;
		static const int DefaultCheckBoxSize = 17;

		virtual void OnMouseClick(const MouseMessage &mouse);
		virtual bool OnKeyUp(const KeyboardMessage &keyboard);

		bool checked;
		Drawing::Point checkBoxLocation;
		Drawing::Point textLocation;
		Drawing::Point checkBoxAbsoluteLocation;
					   
		CheckedChangedEvent checkedChangedEvent;

		Label *label;
	};
}

#endif