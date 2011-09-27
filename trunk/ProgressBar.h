#ifndef __OSHGUI_PROGRESSBAR_H__
#define __OSHGUI_PROGRESSBAR_H__

#include "Control.h"
#include "Panel.h"

namespace OSHGui
{
	/**
	 * Stellt ein Statusanzeige-Steuerlement dar.
	 */
	class ProgressBar : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		ProgressBar(Control *parent = 0);
	
		/**
		 * Legt den minimalen Wert fest.
		 *
		 * @param min
		 */
		void SetMin(unsigned int min);
		/**
		 * Gibt den minimalen Wert zur�ck.
		 *
		 * @return der minimale Wert
		 */
		int GetMin() const;
		/**
		 * Legt den maximalen Wert fest.
		 *
		 * @param max
		 */
		void SetMax(unsigned int max);
		/**
		 * Gibt den maximalen Wert zur�ck.
		 *
		 * @return der maximale Wert
		 */
		int GetMax() const;
		/**
		 * Legt die aktuelle Position fest.
		 *
		 * @param position
		 */
		void SetPosition(unsigned int position);
		/**
		 * Gibt die aktuelle Position zur�ck.
		 *
		 * @return die Position
		 */
		int GetPosition() const;
		/**
		 * Legt die Farbe des Fortschrittsbalken fest.
		 *
		 * @param color
		 */
		void SetBarColor(const Drawing::Color &color);
		/**
		 * Gibt die Farbe des Fortschrittsbalken zur�ck.
		 *
		 * @return die Farbe
		 */
		const Drawing::Color& GetBarColor() const;

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
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		void Adjust();
	
		Drawing::Color barColor;
		unsigned int position,
					 min,
					 max;
	};
}

#endif