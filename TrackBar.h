#ifndef __OSHGUI_TRACKBAR_H__
#define __OSHGUI_TRACKBAR_H__

#include "Control.h"

#define TRACKBAR_SLIDER_WIDTH 8
#define TRACKBAR_SLIDER_HEIGHT 16

namespace OSHGui
{
	/**
	 * Stellt eine Standardtrackleiste dar.
	 */
	class TrackBar : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		TrackBar(Control *parent);
		
		/**
		 * �berpr�ft, ob das Steuerelement den Fokus �bernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus();
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		/**
		 * Legt den Wertebereich (min - max) fest.
		 *
		 * @param min
		 * @param max
		 */
		void SetRange(int min, int max);
		/**
		 * Legt den aktuellen Wert fest.
		 *
		 * @param value
		 */
		void SetValue(int value);
		/**
		 * Gibt den aktuellen Wert zur�ck.
		 *
		 * @return der aktuelle Wert
		 */
		int GetValue();

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
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);

	protected:
		virtual void SetValueInternal(int value);
		virtual int ValueFromPosition(int position);
	
		int min,
			max,
			value,
			sliderMiddle;
		bool pressed;
		
		Drawing::Rectangle sliderRect;
	};
}

#endif