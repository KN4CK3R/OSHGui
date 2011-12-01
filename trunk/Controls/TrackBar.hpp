#ifndef OSHGUI_TRACKBAR_HPP_
#define OSHGUI_TRACKBAR_HPP_

#include "Control.hpp"

namespace OSHGui
{
	/**
	 * Stellt eine Standardtrackleiste dar.
	 */
	class OSHGUI_EXPORT TrackBar : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		TrackBar();
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
		 * Gibt den aktuellen Wert zur�ck.
		 *
		 * @return der aktuelle Wert
		 */
		int GetValue() const;
		/**
		 * Ruft das KeyDownEvent f�r das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyDownEvent& GetKeyDownEvent();
		/**
		 * Ruft das KeyPressEvent f�r das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyPressEvent& GetKeyPressEvent();
		/**
		 * Ruft das KeyUpEvent f�r das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyUpEvent& GetKeyUpEvent();
		/**
		 * Ruft das ScrollEvent f�r das Steuerelement ab.
		 *
		 * @return scrollEvent
		 */
		ScrollEvent& GetScrollEvent();
		
		/**
		 * �berpr�ft, ob das Steuerelement den Fokus �bernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus() const;
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
		/**
		 * Rechnet die Position des angegeben Bildschirmpunkts in Clientkoordinaten um.
		 *
		 * @param point
		 * @return der neue Punkt
		 */
		virtual const Drawing::Point PointToClient(const Drawing::Point &point) const;

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
		virtual bool ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
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

		KeyDownEvent keyDownEvent;
		KeyPressEvent keyPressEvent;
		KeyUpEvent keyUpEvent;
		ScrollEvent scrollEvent;
	};
}

#endif