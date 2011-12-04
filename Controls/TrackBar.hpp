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
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		TrackBar();
		~TrackBar();

		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size);
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
		ScrollEvent& GetScrollEvent() const;
		
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
		virtual void CalculateAbsoluteLocation();
		/**
		 * Rechnet die Position des angegeben Bildschirmpunkts in Clientkoordinaten um.
		 *
		 * @param point
		 * @return der neue Punkt
		 */
		virtual const Drawing::Point PointToClient(const Drawing::Point &point) const;

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);

	protected:
		static const Drawing::Size TrackBarSliderSize;
		static const Drawing::Size TrackBarSize;

		virtual void SetValueInternal(int value);
		virtual int ValueFromPosition(int position) const;
	
		int min,
			max,
			value,
			sliderMiddle;
		bool pressed;
		
		Drawing::Point sliderLocation;
		Drawing::Point sliderAbsoluteLocation;

		KeyDownEvent keyDownEvent;
		KeyPressEvent keyPressEvent;
		KeyUpEvent keyUpEvent;
		ScrollEvent scrollEvent;
	};
}

#endif