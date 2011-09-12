#ifndef __OSHGUI_FORM_H__
#define __OSHGUI_FORM_H__

#include "Control.h"
#include "Misc\TextHelper.h"

namespace OSHGui
{
	/**
	 * Stellt ein Fenster dar, das die Benutzeroberfläche bildet.
	 */
	class Form : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		Form();
		virtual ~Form();
		void Dispose();

		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		/**
		 * Legt den Text fest.
		 *
		 * @param text
		 */
		void SetText(const Misc::UnicodeString &text);
		/**
		 * Gibt den Text zurück.
		 *
		 * @return der Text
		 */
		const Misc::UnicodeString& GetText();
		
		/**
		 * Zeigt die Form an.
		 */
		void Show();

		/**
		 * Schließt die Form.
		 */
		void Close();

		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird außerdem für alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();

		/**
		 * Verarbeitet ein Event und gibt es wenn nötig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		Event::NextEventTypes ProcessEvent(const std::shared_ptr<Event> &event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		void Render(const std::shared_ptr<Drawing::IRenderer> &renderer);

	protected:
		Misc::TextHelper textHelper;
		Drawing::Rectangle captionBar,
						   closeRect;
		bool drag;
	};
}

#endif