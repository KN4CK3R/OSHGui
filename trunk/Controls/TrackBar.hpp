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
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size);
		/**
		 * Legt den minimalen Wert f�r die Schiebereglerposition auf der TrackBar fest.
		 *
		 * @param min
		 */
		void SetMinimum(int minimum);
		/**
		 * Ruft den minimalen Wert f�r die Schiebereglerposition auf der TrackBar ab.
		 *
		 * @return min
		 */
		int GetMinimum() const;
		/**
		 * Legt den maximalen Wert f�r die Schiebereglerposition auf der TrackBar fest.
		 *
		 * @param min
		 */
		void SetMaximum(int maximum);
		/**
		 * Ruft den maximalen Wert f�r die Schiebereglerposition auf der TrackBar ab.
		 *
		 * @return min
		 */
		int GetMaximum() const;
		/**
		 * Legt die Anzahl der Positionen zwischen den Teilstrichen fest.
		 *
		 * @param tickFrequency
		 */
		void SetTickFrequency(int tickFrequency);
		/**
		 * Ruft die Anzahl der Positionen zwischen den Teilstrichen ab
		 *
		 * @return tickFrequency
		 */
		int GetTickFrequency() const;
		/**
		 * Legt den aktuellen Wert fest.
		 *
		 * @param value
		 */
		void SetValue(int value);
		/**
		 * Ruft den aktuellen Wert ab.
		 *
		 * @return der aktuelle Wert
		 */
		int GetValue() const;
		/**
		 * Ruft das ValueChangedEvent f�r das Steuerelement ab.
		 *
		 * @return valueChangedEvent
		 */
		ValueChangedEvent& GetValueChangedEvent();
		
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation();

		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);

	protected:
		static const Drawing::Size SliderSize;
		static const Drawing::Size DefaultSize;
		static const int DefaultTickOffset = 7;

		virtual void SetValueInternal(int value);

		virtual void OnMouseDown(const MouseMessage &mouse);
		virtual void OnMouseUp(const MouseMessage &mouse);
		virtual void OnMouseClick(const MouseMessage &mouse);
		virtual void OnMouseMove(const MouseMessage &mouse);
		virtual bool OnKeyDown(const KeyboardMessage &keyboard);
	
		int minimum;
		int maximum;
		int value;
		int tickFrequency;
		int pixelsPerTick;
		bool drag;
		
		Drawing::Point sliderLocation;
		Drawing::Point sliderAbsoluteLocation;

		ValueChangedEvent valueChangedEvent;
	};
}

#endif