#ifndef OSHGUI_PANEL_HPP_
#define OSHGUI_PANEL_HPP_

#include "ContainerControl.hpp"

namespace OSHGui
{
	/**
	 * Wird zum Gruppieren von Auflistungen von Steuerelementen verwendet.
	 */
	class OSHGUI_EXPORT Panel : public ContainerControl
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param name eindeutiger Name des Steuerelements
		 * @param location Position des Steuerelements
		 * @param size Gr��e des Steuerelements
		 */
		Panel(const Misc::AnsiString &name, const Drawing::Point &location, const Drawing::Size &size);
		
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
		
		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird au�erdem f�r alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();

		/**
		 * Verarbeitet ein Event und gibt es wenn n�tig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		//virtual bool ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
	};
}

#endif