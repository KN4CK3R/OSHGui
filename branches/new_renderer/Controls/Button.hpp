/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_BUTTON_HPP
#define OSHGUI_BUTTON_HPP

#include "Control.hpp"
#include "Label.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein Schaltflächen-Steuerelement dar.
	 */
	class OSHGUI_EXPORT Button : public Control
	{
	public:
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		Button();

		/**
		 * Legt fest, ob sich die Größe des Steuerelements automatisch an dessen Inhalt anpasst.
		 *
		 * \param autoSize
		 */
		virtual void SetAutoSize(bool autoSize) override;
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * \param size
		 */
		virtual void SetSize(const Drawing::SizeF &size) override;
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
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * \param point
		 * \return ja / nein
		 */
		virtual bool Intersect(const Drawing::PointF &point) const override;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation() override;

		virtual void DrawSelf(Drawing::RenderContext &context) override;

	protected:
		virtual void CalculateLabelLocation();
		
		virtual void PopulateGeometry();

		virtual bool OnKeyUp(const KeyboardMessage &keyboard) override;

		std::unique_ptr<Label> label;

	private:
		static const Drawing::SizeF DefaultSize;
		static const Drawing::PointF DefaultLabelOffset;
	};
}

#endif