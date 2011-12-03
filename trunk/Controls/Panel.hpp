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
		 */
		Panel();
		
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
		
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);

	private:
		static const Drawing::Size DefaultSize;
	};
}

#endif