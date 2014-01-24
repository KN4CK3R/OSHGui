/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_CHECKBOX_HPP
#define OSHGUI_CHECKBOX_HPP

#include "Control.hpp"

namespace OSHGui
{
	class Label;

	/**
	 * Tritt ein, wenn sich der Wert der Checked-Eigenschaft ändert.
	 */
	typedef Event<void(Control*)> CheckedChangedEvent;
	typedef EventHandler<void(Control*)> CheckedChangedEventHandler;
	
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

		/**
		 * Legt den checked-Status fest.
		 *
		 * \param checked
		 */
		virtual void SetChecked(bool checked);
		/**
		 * Gibt den checked-Status zurück.
		 *
		 * \return ja / nein
		 */
		virtual bool GetChecked() const;
		/**
		 * Legt den Text fest. Falls autoSize = true, wird die Größe automatisch angepasst.
		 *
		 * \param text
		 */
		void SetText(const Misc::AnsiString &text);
		/**
		 * Ruft den Text ab.
		 *
		 * \return der Text
		 */
		const Misc::AnsiString& GetText() const;
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * \param font
		 */
		virtual void SetFont(const Drawing::FontPtr &font) override;
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * \param color
		 */
		virtual void SetForeColor(Drawing::Color color) override;
		/**
		 * Ruft das CheckedChangeEvent für das Steuerelement ab.
		 *
		 * \return checkedChangeEvent
		 */
		CheckedChangedEvent& GetCheckedChangedEvent();
		
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation() override;

		virtual void DrawSelf(Drawing::RenderContext &context) override;
	
	protected:
		static const Drawing::PointF DefaultLabelOffset;
		static const int DefaultCheckBoxSize = 17;

		virtual void PopulateGeometry() override;

		virtual void OnMouseClick(const MouseMessage &mouse) override;
		virtual bool OnKeyUp(const KeyboardMessage &keyboard) override;

		bool checked;
		Drawing::PointF checkBoxLocation;
		Drawing::PointF textLocation;
		Drawing::PointF checkBoxAbsoluteLocation;
		
		CheckedChangedEvent checkedChangedEvent;

		std::unique_ptr<Label> label;
	};
}

#endif