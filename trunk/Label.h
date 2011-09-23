#ifndef __OSHGUI_LABEL_H__
#define __OSHGUI_LABEL_H__

#include "Control.h"
#include "Misc\TextHelper.h"

namespace OSHGui
{
	/**
	 * Stellt ein Label-Steuerelement dar.
	 */
	class Label : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		Label(Control *parent = 0);
		
		/**
		 * Legt den Text fest. Falls autoSize = true, wird die Gr��e automatisch angepasst.
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
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point) const;
		
		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird au�erdem f�r alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();
		
		/**
		 * Verarbeitet ein {@link Event Event} und gibt es wenn n�tig an Kindelement weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		bool pressed;
		Misc::TextHelper textHelper;
	};
}

#endif