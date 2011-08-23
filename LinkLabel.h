#ifndef __OSHGUI_LINKLABEL_H__
#define __OSHGUI_LINKLABEL_H__

#include "Label.h"

namespace OSHGui
{
	/**
	 * Diese Klasse repräsentiert ein LinkLabel.
	 */
	class LinkLabel : public Label
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		LinkLabel(Control *parent = NULL);
		
		/**
		 * Verarbeitet ein Event und gibt es wenn nötig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
	};
}

#endif