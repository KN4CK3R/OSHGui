#ifndef OSHGUI_BUTTON_HPP_
#define OSHGUI_BUTTON_HPP_

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
		static const Drawing::Size DefaultButtonSize;

		/**
		 * Konstruktor der Klasse.
		 *
		 * @param name eindeutiger Name des Steuerelements
		 * @param location Position des Steuerelements
		 * @param size Größe des Steuerelements
		 * @param text Text des Steuerelemts
		 */
		Button(const Misc::AnsiString &name, const Drawing::Point &location, const Drawing::Size &size, const Misc::AnsiString &text);
		virtual ~Button();

		/**
		 * Legt fest, ob sich die Größe des Steuerelements automatisch an dessen Inhalt anpasst.
		 *
		 * @param autoSize
		 */
		virtual void SetAutoSize(bool autoSize);
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
		void SetFont(const std::shared_ptr<Drawing::IFont> &font);
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetForeColor(Drawing::Color color);
		
		/**
		 * Überprüft, ob das Steuerelement den Fokus übernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus() const;
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
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird außerdem für alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();
		
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);

	protected:
		Label *label;
	};
}

#endif