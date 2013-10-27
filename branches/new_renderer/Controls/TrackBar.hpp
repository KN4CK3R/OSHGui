/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_TRACKBAR_HPP
#define OSHGUI_TRACKBAR_HPP

#include "Control.hpp"

namespace OSHGui
{
	/**
	 * Tritt auf, wenn der TrackBar-Schieberegler verschoben wird.
	 */
	typedef Event<void(Control*)> ValueChangedEvent;
	typedef EventHandler<void(Control*)> ValueChangedEventHandler;

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
		virtual ~TrackBar();

		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * \param size
		 */
		virtual void SetSize(const Drawing::SizeF &size) override;
		/**
		 * Legt den minimalen Wert für die Schiebereglerposition auf der TrackBar fest.
		 *
		 * \param minimum
		 */
		void SetMinimum(int minimum);
		/**
		 * Ruft den minimalen Wert für die Schiebereglerposition auf der TrackBar ab.
		 *
		 * \return minimum
		 */
		int GetMinimum() const;
		/**
		 * Legt den maximalen Wert für die Schiebereglerposition auf der TrackBar fest.
		 *
		 * \param maximum
		 */
		void SetMaximum(int maximum);
		/**
		 * Ruft den maximalen Wert für die Schiebereglerposition auf der TrackBar ab.
		 *
		 * \return maximum
		 */
		int GetMaximum() const;
		/**
		 * Legt die Anzahl der Positionen zwischen den Teilstrichen fest.
		 *
		 * \param tickFrequency
		 */
		void SetTickFrequency(int tickFrequency);
		/**
		 * Ruft die Anzahl der Positionen zwischen den Teilstrichen ab
		 *
		 * \return tickFrequency
		 */
		int GetTickFrequency() const;
		/**
		 * Legt den aktuellen Wert fest.
		 *
		 * \param value
		 */
		void SetValue(int value);
		/**
		 * Ruft den aktuellen Wert ab.
		 *
		 * \return der aktuelle Wert
		 */
		int GetValue() const;
		/**
		 * Ruft das ValueChangedEvent für das Steuerelement ab.
		 *
		 * \return valueChangedEvent
		 */
		ValueChangedEvent& GetValueChangedEvent();
		
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * \param point
		 * \return ja / nein
		 */
		virtual bool Intersect(const Drawing::PointF &point) const override;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation() override;

	protected:
		static const Drawing::SizeF SliderSize;
		static const Drawing::SizeF DefaultSize;
		static const int DefaultTickOffset = 7;

		void SetValueInternal(int value);

		virtual void PopulateGeometry() override;

		virtual void OnMouseDown(const MouseMessage &mouse) override;
		virtual void OnMouseUp(const MouseMessage &mouse) override;
		virtual void OnMouseClick(const MouseMessage &mouse) override;
		virtual void OnMouseMove(const MouseMessage &mouse) override;
		virtual void OnMouseScroll(const MouseMessage &mouse) override;
		virtual bool OnKeyDown(const KeyboardMessage &keyboard) override;
	
		int minimum;
		int maximum;
		int value;
		int tickFrequency;
		float pixelsPerTick;
		bool drag;
		
		Drawing::PointF sliderLocation;
		Drawing::PointF sliderAbsoluteLocation;

		ValueChangedEvent valueChangedEvent;
	};
}

#endif