#ifndef OSHGUI_LABEL_HPP_
#define OSHGUI_LABEL_HPP_

#include "Control.hpp"
#include "..\Misc\TextHelper.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein Label-Steuerelement dar.
	 */
	class OSHGUI_EXPORT Label : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		Label();
		
		/**
		 * Legt den Text fest. Falls autoSize = true, wird die Größe automatisch angepasst.
		 *
		 * @param text
		 */
		void SetText(const Misc::UnicodeString &text);
		/**
		 * Ruft den Text ab.
		 *
		 * @return der Text
		 */
		const Misc::UnicodeString& GetText();
		
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point) const;
		
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
		Misc::TextHelper textHelper;
	};
}

#endif