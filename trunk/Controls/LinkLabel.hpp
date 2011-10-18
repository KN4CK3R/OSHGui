#ifndef OSHGUI_LINKLABEL_H_
#define OSHGUI_LINKLABEL_H_

#include "Label.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein Label-Steuerelement dar, das Links anzeigen kann.
	 */
	class LinkLabel : public Label
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		LinkLabel(Control *parent = 0);
		~LinkLabel();
		
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
	};
}

#endif