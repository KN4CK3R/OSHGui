/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
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
	 * Tritt ein, wenn sich der Wert der Checked-Eigenschaft �ndert.
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
		 * @param checked
		 */
		virtual void SetChecked(bool checked);
		/**
		 * Gibt den checked-Status zur�ck.
		 *
		 * @return ja / nein
		 */
		virtual bool GetChecked() const;
		/**
		 * Legt den Text fest. Falls autoSize = true, wird die Gr��e automatisch angepasst.
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
		virtual void SetFont(const std::shared_ptr<Drawing::IFont> &font) override;
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetForeColor(Drawing::Color color) override;
		/**
		 * Ruft das CheckedChangeEvent f�r das Steuerelement ab.
		 *
		 * @return checkedChangeEvent
		 */
		CheckedChangedEvent& GetCheckedChangedEvent();
		
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::PointF &point) const override;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation() override;
		
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer) override;

		virtual void DrawSelf(Drawing::RenderContext &context) override;
	
	protected:
		static const Drawing::SizeF DefaultLabelOffset;
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