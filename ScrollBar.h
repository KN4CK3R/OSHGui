#ifndef __OSHGUI_SCROLLBAR_H__
#define __OSHGUI_SCROLLBAR_H__

#include "Control.h"

#define SCROLLBAR_MIN_SLIDER_HEIGHT 25
#define SCROLLBAR_DEFAULT_BOUNDS_WIDTH 14
#define SCROLLBAR_DEFAULT_BUTTON_WIDTH 14
#define SCROLLBAR_DEFAULT_BUTTON_HEIGHT 18

namespace OSHGui
{
	/**
	 * Implementiert die Basisfunktionen eines Schiebeleisten-Steuerelements.
	 */
	class ScrollBar : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		ScrollBar(Control *parent);
		
		/**
		 * Legt die Anzahl der scrollbaren Elemente fest.
		 *
		 * @param range
		 */
		void SetRange(int range);
		/**
		 * Gibt die aktuelle Position des ScrollButtons zurück.
		 *
		 * @return die Position
		 */
		int GetPosition() const;
		/**
		 * Legt die Größe des sichtbaren Ausschnitts fest.
		 *
		 * @param pageSize
		 */
		void SetPageSize(int pageSize);
		/**
		 * Gibt die Größe des sichtbaren Ausschnitts zurück.
		 *
		 * @return die Größe des sichtbaren Ausschnitts
		 */
		int GetPageSize() const;

		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		/**
		 * Verschiebt den ScrollButton, damit er den Index anzeigt.
		 *
		 * @return false, falls die ScrollBar nicht aktiv ist
		 */
		bool ShowItem(int index);

		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird außerdem für alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();

		/**
		 * Wandelt den Punkt in ClientKoordinaten um.
		 *
		 * @param point
		 * @return der neue Punkt
		 */
		virtual Drawing::Point PointToClient(const Drawing::Point &point);
		
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
		void Scroll(int delta);
		void UpdateSliderRect();
		void Capture();
	
		int position,
			range,
			pageSize;
		bool drag;
		
		Drawing::Rectangle upButtonRect,
						   downButtonRect,
						   trackRect,
						   sliderRect;
				  
		long delayTimestamp;
	};
}

#endif