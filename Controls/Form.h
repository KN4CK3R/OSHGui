#ifndef OSHGUI_FORM_H_
#define OSHGUI_FORM_H_

#include <functional>
#include "Control.h"
#include "..\Misc\TextHelper.h"

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

		bool IsModal() const;

		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point) const;
		
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
		const Misc::UnicodeString& GetText() const;
		
		/**
		 * Zeigt die Form an.
		 *
		 * @param instance die aktuelle Instanz dieser Form
		 */
		void Show(const std::shared_ptr<Form> &instance);
		/**
		 * Zeigt die Form modal an.
		 *
		 * @param instance die aktuelle Instanz dieser Form
		 */
		void ShowModal(const std::shared_ptr<Form> &instance);
		/**
		 * Zeigt die Form modal an.
		 *
		 * @param instance die aktuelle Instanz dieser Form
		 * @param func diese Funktion wird ausgeführt, wenn die Form geschlossen wird (kann 0 sein)
		 */
		void ShowModal(const std::shared_ptr<Form> &instance, const std::function<void()> &closeFunction);

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
		IEvent::NextEventTypes ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		void Render(Drawing::IRenderer *renderer);

	protected:
		Misc::TextHelper textHelper;
		Drawing::Rectangle captionBar,
						   closeRect;
		bool drag;

		std::weak_ptr<Form> instance;
	};
}

#endif