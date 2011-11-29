#ifndef OSHGUI_LINKLABEL_HPP_
#define OSHGUI_LINKLABEL_HPP_

#include "Label.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein Label-Steuerelement dar, das Links anzeigen kann.
	 */
	class OSHGUI_EXPORT LinkLabel : public Label
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		LinkLabel();
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