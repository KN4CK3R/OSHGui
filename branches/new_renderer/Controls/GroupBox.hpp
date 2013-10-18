/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_GROUPBOX_HPP
#define OSHGUI_GROUPBOX_HPP

#include "Panel.hpp"

namespace OSHGui
{
	class Label;

	/**
	 * Stellt ein Steuerelement dar, dass einen Rahmen um eine Gruppe
	 * von Steuerlementen anzeigt, der eine Beschriftung enthalten kann.
	 */
	class OSHGUI_EXPORT GroupBox : public Panel
	{
	public:
		using Panel::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		GroupBox();
		virtual ~GroupBox();

		/**
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::SizeF &size) override;
		/**
		 * Legt den Text fest.
		 *
		 * @param text
		 */
		void SetText(const Misc::AnsiString &text);
		/**
		 * Gibt den Text zur�ck.
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
		 * Gibt eine Liste der untergeordneten Steuerelemente zur�ck.
		 *
		 * @return parent
		 */
		virtual const std::deque<Control*>& GetControls() const override;
		
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::PointF &point) const override;
		/**
		 * F�gt ein untergeordnetes Steuerelement hinzu.
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
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer) override;

		virtual void DrawSelf(Drawing::RenderContext &context) override;

	protected:
		virtual void PopulateGeometry() override;
		
	private:
		Label *captionLabel;
		Panel *containerPanel;
	};
}

#endif