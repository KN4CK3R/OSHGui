#ifndef __OSHGUI_TRACKBAR_H__
#define __OSHGUI_TRACKBAR_H__

#include "Event\EventHandler.h"
#include "Control.h"

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
		~TrackBar();

		/**
		 * Legt den Wertebereich (min - max) fest.
		 *
		 * @param min
		 * @param max
		 */
		void SetRange(int min, int max);
		/**
		 * Ruft den Wertebereich ab.
		 *
		 * @param min
		 * @param max
		 */
		void GetRange(int *min, int *max) const;
		/**
		 * Legt den aktuellen Wert fest.
		 *
		 * @param value
		 */
		void SetValue(int value);
		/**
		 * Gibt den aktuellen Wert zurück.
		 *
		 * @return der aktuelle Wert
		 */
		int GetValue() const;
		/**
		 * Ruft das ScrollEvent für das Steuerelement ab.
		 *
		 * @return scrollEvent
		 */
		ScrollEvent& GetScrollEvent();
		
		/**
		 * Überprüft, ob das Steuerelement den Fokus übernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus() const;
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point) const;
		/**
		 * Rechnet die Position des angegeben Bildschirmpunkts in Clientkoordinaten um.
		 *
		 * @param point
		 * @return der neue Punkt
		 */
		virtual const Drawing::Point PointToClient(const Drawing::Point &point) const;

		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird außerdem für alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();
		
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

	protected:
		virtual void SetValueInternal(int value);
		virtual int ValueFromPosition(int position) const;
	
		static const int trackbarSliderWidth;
		static const int trackbarSliderHeight;
	
		int min,
			max,
			value,
			sliderMiddle;
		bool pressed;
		
		Drawing::Rectangle sliderRect;

		ScrollEvent scrollEvent;
	};
}

#endif