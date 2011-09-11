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
		ProgressBar(const std::shared_ptr<Control> &parent = 0);
	
		/**
		 * Legt den minimalen Wert fest.
		 *
		 * @param min
		 */
		void SetMin(unsigned int min);
		/**
		 * Gibt den minimalen Wert zurück.
		 *
		 * @return der minimale Wert
		 */
		int GetMin();
		/**
		 * Legt den maximalen Wert fest.
		 *
		 * @param max
		 */
		void SetMax(unsigned int max);
		/**
		 * Gibt den maximalen Wert zurück.
		 *
		 * @return der maximale Wert
		 */
		int GetMax();
		/**
		 * Legt die aktuelle Position fest.
		 *
		 * @param position
		 */
		void SetPosition(unsigned int position);
		/**
		 * Gibt die aktuelle Position zurück.
		 *
		 * @return die Position
		 */
		int GetPosition();
		/**
		 * Legt die Farbe des Fortschrittsbalken fest.
		 *
		 * @param color
		 */
		void SetBarColor(const Drawing::Color &color);
		/**
		 * Gibt die Farbe des Fortschrittsbalken zurück.
		 *
		 * @return die Farbe
		 */
		const Drawing::Color& GetBarColor();

		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point);

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
		virtual void Render(const std::shared_ptr<Drawing::IRenderer> &renderer);
		
	protected:
		void Adjust();
	
		Drawing::Color barColor;
		unsigned int position,
					 min,
					 max;
	};
}

#endif