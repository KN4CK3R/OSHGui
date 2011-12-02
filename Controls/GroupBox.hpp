#ifndef OSHGUI_GROUPBOX_HPP_
#define OSHGUI_GROUPBOX_HPP_

#include "Control.hpp"

namespace OSHGui
{
	class Label;

	/**
	 * Stellt ein Steuerelement dar, dass einen Rahmen um eine Gruppe
	 * von Steuerlementen anzeigt, der eine Beschriftung enthalten kann.
	 */
	class OSHGUI_EXPORT GroupBox : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		GroupBox();
		virtual ~GroupBox();

		/**
		 * Legt den Text fest.
		 *
		 * @param text
		 */
		void SetText(const Misc::AnsiString &text);
		/**
		 * Gibt den Text zurück.
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
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
	private:
		Label *label;
	};
}

#endif