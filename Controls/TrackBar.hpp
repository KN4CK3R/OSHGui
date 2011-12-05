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
		 * Legt den minimalen Wert für die Schiebereglerposition auf der TrackBar fest.
		 *
		 * @param min
		 */
		void SetMinimum(int minimum);
		/**
		 * Ruft den minimalen Wert für die Schiebereglerposition auf der TrackBar ab.
		 *
		 * @return min
		 */
		int GetMinimum() const;
		/**
		 * Legt den maximalen Wert für die Schiebereglerposition auf der TrackBar fest.
		 *
		 * @param min
		 */
		void SetMaximum(int maximum);
		/**
		 * Ruft den maximalen Wert für die Schiebereglerposition auf der TrackBar ab.
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
		 * Ruft das ValueChangedEvent für das Steuerelement ab.
		 *
		 * @return valueChangedEvent
		 */
		ValueChangedEvent& GetValueChangedEvent();
		
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
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
		static const int DefaultTickOffset = 7;

		virtual void SetValueInternal(int value);

		virtual void OnMouseDown(const MouseMessage &mouse);
		virtual void OnMouseUp(const MouseMessage &mouse);
		virtual void OnMouseClick(const MouseMessage &mouse);
		virtual void OnMouseMove(const MouseMessage &mouse);
		virtual void OnKeyDown(const KeyboardMessage &keyboard);
	
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